#include "TCore.h"
#include "ICoreStd.h"
void TCore::CreateSamplerState()
{
    D3D11_SAMPLER_DESC descSamp;
    descSamp.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    descSamp.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    descSamp.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    descSamp.MipLODBias = 0;
    descSamp.MaxAnisotropy = 16;

    descSamp.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    descSamp.ComparisonFunc = D3D11_COMPARISON_NEVER;

    descSamp.BorderColor[0] = 1.0f;
    descSamp.BorderColor[1] = 0.0f;
    descSamp.BorderColor[2] = 0.0f;
    descSamp.BorderColor[3] = 1.0f;
    descSamp.MinLOD = 0;
    descSamp.MaxLOD = D3D11_FLOAT32_MAX;
    m_pDevice->CreateSamplerState(&descSamp, m_pSamplerState.GetAddressOf());    
}
void  TCore::CreateBlendState()
{
    // alpha blending
    D3D11_BLEND_DESC bsd;
    ZeroMemory(&bsd, sizeof(bsd));
    bsd.RenderTarget[0].BlendEnable = true;
    bsd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bsd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    bsd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    // ���ĺ��� ���� - �ҽ�(float4(0,1,0,0.5f)), ���(1,0,0,1)
    //finalcolor(RGB) = SrcColor* SrcBlend   + DestColor*DestBlend
    //           = Scrcolor* alphaValue + DestColor * (1.0f-alphaValue)
    //           = 0,1,0 * 0.5f + 1,0,0 * (1.0f-0.5f)
    // //        ���� t= 0.0f; ��� => ��游 ���´�.
    // //        ���� t= 1.0f; ��� => �ҽ��� ���´�.
    //           ���� t= 0.5f; ��� - >�ҽ��÷�*0.5 + ����÷�*0.5f
    //           = 0,1,0 * t + 1,0,0 * (1.0f-t)
    //           =  r*t,b*t,b*t + r*(1.0f-t),b*(1.0f-t),b*(1.0f-t)

    // (A)
    bsd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bsd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bsd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

    bsd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    m_pDevice->CreateBlendState(&bsd, &m_AlphaBlend);
}
bool  TCore::Init() { return true; }
bool  TCore::Frame() { return true; }
bool  TCore::Render() { return true; }
bool  TCore::Release() { return true; }
bool  TCore::EngineInit()
{
    TDevice::Init();

    ICore::g_pDevice = m_pDevice;
    ICore::g_pContext = m_pImmediateContext;

    CreateBlendState();
    CreateSamplerState();

    I_Tex.Set(m_pDevice, m_pImmediateContext);
    I_Shader.Set(m_pDevice, m_pImmediateContext);

    m_GameTimer.Init();    
    TInput::GetInstance().Init();  

    m_pDefaultCamera = std::make_shared<TCamera>();
    m_pDefaultCamera->Init();
    TVector3 vPos = { 0,50,-1 };
    TVector3 vUp = { 0,0,0 };
    m_pDefaultCamera->CreateLookAt(vPos, vUp);
    m_pDefaultCamera->CreatePerspectiveFov(
        T_PI * 0.25, (float)g_dwWindowWidth / (float)g_dwWindowHeight,
                      1.0f, 10000.0f);

    ICore::g_pMainCamera = m_pDefaultCamera.get();

    I_Writer.Init();    
    if (m_pSwapChain)
    {
        IDXGISurface1* pBackBuffer;
        HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface1),
            (LPVOID*)&pBackBuffer);
        if (SUCCEEDED(hr))
        {
            I_Writer.Create(pBackBuffer);
        }
        if (pBackBuffer) pBackBuffer->Release();
    }

	Init();    
	return true;
}
bool  TCore::EngineFrame()
{
    m_GameTimer.Frame();
    TInput::GetInstance().Frame();
    ICore::g_pMainCamera->Frame();
    TDevice::Frame();
    I_Writer.Frame();
	Frame();
	return true;
}
bool  TCore::EngineRender()
{
    TDevice::PreRender();
    m_pImmediateContext->OMSetBlendState(m_AlphaBlend, 0, -1);
    m_pImmediateContext->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());
    //m_pSamplerState.Get();
	Render();

    ICore::g_pMainCamera->Render();
    m_GameTimer.Render();
    TInput::GetInstance().Render();

    I_Writer.Render();
    TDevice::PostRender();
	return true;
}
bool  TCore::EngineRelease()
{
	Release();

    if (m_AlphaBlend)m_AlphaBlend->Release();
   
    m_GameTimer.Release();
    TInput::GetInstance().Release();
    m_pDefaultCamera->Release();
    I_Writer.Release();
    TDevice::Release();
	return true;
}
bool TCore::Run()
{
    EngineInit();
    MSG msg = { 0 };
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // ���ӷ����� ó��
            if (!EngineFrame() || !EngineRender())
            {
                break;
            }
        }
    }
    EngineRelease();
    return true;
}