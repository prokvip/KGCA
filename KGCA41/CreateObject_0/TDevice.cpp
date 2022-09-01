#include "TDevice.h"
bool		TDevice::Init()
{ 
    return true;
}
bool		TDevice::Frame()
{
    return true;
}
bool		TDevice::Render()
{  
    return true;
}
bool		TDevice::Release()
{
    return true;
}

bool		TDevice::TCoreInit()
{
    HRESULT hr;
    if (FAILED(hr = CreateDXGIDevice()))
    {
        return false;
    }
    if (FAILED(hr = CreateDevice()))
    {
        return false;
    }
    if (FAILED(hr = CreateSwapChain()))
    {
        return false;
    }
    if (FAILED(hr = CreateRenderTargetView()))
    {
        return false;
    }
    CreateViewport();

    return Init();
}
bool		TDevice::TCoreFrame()
{
    return Frame();
}
bool		TDevice::TCorePreRender()
{
    m_pImmediateContext->OMSetRenderTargets(1, &m_pRTV, NULL);
    float color[4] = { 1.0f,1.0f,1.0f,1.0f };
    m_pImmediateContext->ClearRenderTargetView(m_pRTV, color);   
    return true;
}
bool		TDevice::TCoreRender()
{
    TCorePreRender();
        Render();
    TCorePostRender();
    return true;
}
bool		TDevice::TCorePostRender()
{
    m_pSwapChain->Present(0, 0);
    return true;
}
bool		TDevice::TCoreRelease()
{
    Release();
    if (m_pd3dDevice) m_pd3dDevice->Release();// 디바이스 객체        
    if (m_pImmediateContext)m_pImmediateContext->Release();
    if (m_pGIFactory)m_pGIFactory->Release();
    if (m_pSwapChain)m_pSwapChain->Release();
    if (m_pRTV) m_pRTV->Release();
    return true;
}


HRESULT TDevice::CreateDevice()
{
    // 1)디바이스 생성
    HRESULT hr;
    D3D_FEATURE_LEVEL pFeatureLevel;
    UINT Flags = 0;
    D3D_FEATURE_LEVEL pFeatureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
    };
    UINT FeatureLevels = 1;
    hr = D3D11CreateDevice(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, pFeatureLevels, 1, D3D11_SDK_VERSION,
        &m_pd3dDevice,// 디바이스 객체
        &pFeatureLevel,
        &m_pImmediateContext
    );

    //m_pd3dDevice  다른 인터페이스 생성시 사용한다.
    //m_pImmediateContext 생성된 인터페이스를 사용(관리,제어)할 때.
    return hr;
}

HRESULT TDevice::CreateDXGIDevice()
{
    // 2)팩토리 생성
    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_pGIFactory);
    return hr;
}

HRESULT TDevice::CreateSwapChain( )
{
    // 3)스왑체인 생성
    // 후면(백)버퍼 생성  -> 전면버퍼
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width  = g_rtClient.right;
    sd.BufferDesc.Height = g_rtClient.bottom;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = g_hWnd;
    sd.Windowed = true;

    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;

    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;

    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    return m_pGIFactory->CreateSwapChain(m_pd3dDevice, &sd, &m_pSwapChain);
}

HRESULT TDevice::CreateRenderTargetView()
{
    HRESULT hr;
    // 4)랜더타켓뷰 생성
    ID3D11Texture2D* pBackBuffer = nullptr;
    m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
    hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRTV);
    pBackBuffer->Release();
    return hr;
}

void TDevice::CreateViewport()
{
    // 5)뷰포트 설정
    D3D11_VIEWPORT vp;
    vp.Width  = g_rtClient.right;
    vp.Height = g_rtClient.bottom;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    m_pImmediateContext->RSSetViewports(1, &vp);
}