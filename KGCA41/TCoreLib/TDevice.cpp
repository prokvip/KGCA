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
    UINT Flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
    Flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
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
        // 3) 렌더타켓 지정하고 ( 컬러가 저장된다.)
        hr = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
        if (FAILED(hr))
        {
            pBackBuffer->Release();
            return false;
        }
        
    }
    pBackBuffer->Release();
    
    // 3) 깊이 버퍼 생성
    // -깊이 버퍼용 텍스터 생성
    // -생성된 깊이 텍스쳐를 깊이버퍼로 활용
    ComPtr<ID3D11Texture2D> depthTexture;
    DXGI_SWAP_CHAIN_DESC scDesc;
    m_pSwapChain->GetDesc(&scDesc);

    D3D11_TEXTURE2D_DESC texDesc =
    {
        scDesc.BufferDesc.Width,
        scDesc.BufferDesc.Height,
        1,1,
        DXGI_FORMAT_R24G8_TYPELESS,//DXGI_FORMAT_D24_UNORM_S8_UINT
        1, 0,
        D3D11_USAGE_DEFAULT, 
        D3D11_BIND_DEPTH_STENCIL,
        0, 0
    };
    hr = m_pDevice->CreateTexture2D(&texDesc, NULL, depthTexture.GetAddressOf());


    D3D11_DEPTH_STENCIL_VIEW_DESC dspDesc;
    ZeroMemory(&dspDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
    dspDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dspDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    hr=m_pDevice->CreateDepthStencilView(depthTexture.Get(), 
        &dspDesc, m_pDepthStencilView.GetAddressOf());
        
        
    m_ViewPort.Width = 800;
    m_ViewPort.Height = 600;
    m_ViewPort.MinDepth = 0.0f;
    m_ViewPort.MaxDepth = 1.0f;
    m_ViewPort.TopLeftX = 0;
    m_ViewPort.TopLeftY = 0;
   
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
    m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), 
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_pImmediateContext->OMSetRenderTargets(1,  &m_pRenderTargetView, 
                                                m_pDepthStencilView.Get());
    m_pImmediateContext->RSSetViewports(1, &m_ViewPort);
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
    if (m_pRenderTargetView)m_pRenderTargetView->Release();
    if (m_pSwapChain)m_pSwapChain->Release();
    if (m_pDevice)m_pDevice->Release();
    if (m_pImmediateContext)m_pImmediateContext->Release();
    return true;
}
