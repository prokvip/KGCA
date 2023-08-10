#include "TDevice.h"
bool  TDevice::Init()
{
    DXGI_SWAP_CHAIN_DESC SwapChainDesc;
    ZeroMemory(&SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
    SwapChainDesc.BufferDesc.Width = 800;
    SwapChainDesc.BufferDesc.Height = 600;
    SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    SwapChainDesc.SampleDesc.Count = 1;
    SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDesc.BufferCount = 1;
    SwapChainDesc.OutputWindow = m_hWnd;
    SwapChainDesc.Windowed = true;

    D3D_DRIVER_TYPE DriverType = D3D_DRIVER_TYPE_HARDWARE;
    UINT Flags = 0;
    D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
    // 1) 디바이스
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        NULL,
        DriverType,
        NULL,
        Flags,
        &pFeatureLevels, 1,
        D3D11_SDK_VERSION,
        &SwapChainDesc,

        &m_pSwapChain,   // 백버퍼인터페이스
        &m_pDevice,      // dx 인터페이스( 생성 )
        NULL,
        &m_pImmediateContext); // dx 인터페이스( 관리 )
    if (FAILED(hr))
    {
        return false;
    }
    // 2) 백버퍼 얻어서
    ID3D11Texture2D* pBackBuffer;
    hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    if (SUCCEEDED(hr))
    {
        // 3) 렌더타켓 지정하고
        hr = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
        if (FAILED(hr))
        {
            pBackBuffer->Release();
            return false;
        }
        m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);
    }
    pBackBuffer->Release();


    m_ViewPort.Width = 800;
    m_ViewPort.Height = 600;
    m_ViewPort.MinDepth = 0.0f;
    m_ViewPort.MaxDepth = 1.0f;
    m_ViewPort.TopLeftX = 0;
    m_ViewPort.TopLeftY = 0;
    m_pImmediateContext->RSSetViewports(1, &m_ViewPort);
    return true;
}
bool  TDevice::Frame()
{
    return true;
}
bool  TDevice::PreRender()
{
    float color[4] = { 0.343f,0.34522f,0.64333f,1 };
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, color);   
    return true;
}
bool  TDevice::PostRender()
{
    HRESULT hr = m_pSwapChain->Present(0, 0);
    if (FAILED(hr))
    {
        return false;
    }
    return true;
}
bool  TDevice::Render()
{
    PreRender();
    PostRender();
    return true;
}
bool  TDevice::Release()
{
    if (m_pSwapChain)m_pSwapChain->Release();
    if (m_pDevice)m_pDevice->Release();
    if (m_pImmediateContext)m_pImmediateContext->Release();
    return true;
}
