#include "Sample.h"
bool		Sample::Init()
{
    // 1)디바이스 생성
    HRESULT hr;
    D3D_FEATURE_LEVEL pFeatureLevel;
    UINT Flags=0;
    D3D_FEATURE_LEVEL pFeatureLevels[] =
    {        
        D3D_FEATURE_LEVEL_11_0, 
    };
    UINT FeatureLevels =1;
    hr = D3D11CreateDevice(
        nullptr, D3D_DRIVER_TYPE_HARDWARE,NULL,0, pFeatureLevels, 1, D3D11_SDK_VERSION,
        &m_pd3dDevice,// 디바이스 객체
        &pFeatureLevel,
        &m_pImmediateContext
    );
    if (FAILED(hr))
    {
        return false;
    }


    // 2)팩토리 생성
    hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_pGIFactory);
    if (FAILED(hr)) return false;
    if (m_pd3dDevice == NULL) return E_FAIL;    

    // 3)스왑체인 생성
    // 후면(백)버퍼 생성  -> 전면버퍼
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = m_rtClient.right;
    sd.BufferDesc.Height = m_rtClient.bottom;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = m_hWnd;
    sd.Windowed     = true;

    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;

    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    hr = m_pGIFactory->CreateSwapChain(m_pd3dDevice,&sd,&m_pSwapChain);
    if (FAILED(hr))
    {
        return false;
    }

    // 4)랜더타켓뷰 생성
    ID3D11Texture2D* pBackBuffer = nullptr;
    m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer );
        m_pd3dDevice->CreateRenderTargetView(pBackBuffer , NULL, &m_pRTV);
    pBackBuffer->Release();

    // 5)뷰포트 설정
    D3D11_VIEWPORT vp;
    vp.Width = m_rtClient.right;
    vp.Height = m_rtClient.bottom;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    m_pImmediateContext->RSSetViewports(1, &vp);
    return true;
}
bool		Sample::Frame()
{
    return true;
}
bool		Sample::Render()
{
    static float fTimer = 0.0f;
    fTimer += 0.0001f;
    m_pImmediateContext->OMSetRenderTargets(1, &m_pRTV, NULL);
    float color[4] = {  cosf(fTimer)*0.5f+0.5f,
                        sinf(fTimer) * 0.5f + 0.5f,
                        cosf(fTimer*2) * 0.5f + 0.5f, 
                         1.0f };
    m_pImmediateContext->ClearRenderTargetView(m_pRTV, color);
    m_pSwapChain->Present(0, 0);
    return true;
}
bool		Sample::Release()
{
    if (m_pd3dDevice) m_pd3dDevice->Release();// 디바이스 객체        
    if (m_pImmediateContext)m_pImmediateContext->Release();
    if (m_pGIFactory)m_pGIFactory->Release();
    if (m_pSwapChain)m_pSwapChain->Release();
    if (m_pRTV) m_pRTV->Release();
    return true;
}

GAME_RUN(P2_CreateWidnow2, 1024, 768)
