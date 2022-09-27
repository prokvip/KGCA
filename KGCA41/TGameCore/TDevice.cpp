#include "TDevice.h"
bool		TDevice::Init()
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

HRESULT TDevice::CreateDevice()
{
    // 1)����̽� ����
    HRESULT hr;
    UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif


    D3D_FEATURE_LEVEL pFeatureLevel;
    D3D_FEATURE_LEVEL pFeatureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
    };
    UINT FeatureLevels = 1;
    hr = D3D11CreateDevice(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, 
        createDeviceFlags, pFeatureLevels, 1, D3D11_SDK_VERSION,
        &m_pd3dDevice,// ����̽� ��ü
        &pFeatureLevel,
        &m_pImmediateContext
    );

    //m_pd3dDevice  �ٸ� �������̽� ������ ����Ѵ�.
    //m_pImmediateContext ������ �������̽��� ���(����,����)�� ��.
    return hr;
}

HRESULT TDevice::CreateDXGIDevice()
{
    // 2)���丮 ����
    /*HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), 
        (void**)&m_pGIFactory);*/
    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory),
        (void**)m_pGIFactory.GetAddressOf()); 
    return hr;
}

HRESULT TDevice::CreateSwapChain( )
{
    // 3)����ü�� ����
    // �ĸ�(��)���� ����  -> �������
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
    return m_pGIFactory->CreateSwapChain(m_pd3dDevice.Get(), 
        &sd, m_pSwapChain.GetAddressOf());
}

HRESULT TDevice::CreateRenderTargetView()
{
    HRESULT hr;
    // 4)����Ÿ�Ϻ� ����
    ID3D11Texture2D* pBackBuffer = nullptr;
    m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
    hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer,
        NULL, m_pRTV.GetAddressOf());
    pBackBuffer->Release();
    return hr;
}

void TDevice::CreateViewport()
{
    // 5)����Ʈ ����
    m_vp.Width  = g_rtClient.right;
    m_vp.Height = g_rtClient.bottom;
    m_vp.TopLeftX = 0;
    m_vp.TopLeftY = 0;
    m_vp.MinDepth = 0.0f;
    m_vp.MaxDepth = 1.0f;
    m_pImmediateContext->RSSetViewports(1, &m_vp);
}