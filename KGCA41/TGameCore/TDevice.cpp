#include "TDevice.h"
HRESULT TDevice::CreateDXResource()
{
    return true;
}
HRESULT TDevice::DeleteDXResource()
{
    return true;
}
HRESULT		TDevice::ResizeDevice(UINT width, UINT height)
{
    HRESULT hr;
    //	윈도우 크기 변경 메시지 검출(WM_SIZE)
    if(m_pd3dDevice == nullptr) return S_OK;       
    g_rtClient.right = width;
    g_rtClient.bottom = height;

    //	현재 설정된 랜더타켓 해제 및 소멸
    DeleteDXResource();
    m_pImmediateContext->OMSetRenderTargets(0, nullptr, NULL);
    m_pRTV.ReleaseAndGetAddressOf();
    m_pDepthStencilView.ReleaseAndGetAddressOf();
    //	변경된 윈도우의 크기를 얻고 백 버퍼의 크기를 재 조정.
    // 백버퍼의 크기를 조정한다.
    DXGI_SWAP_CHAIN_DESC CurrentSD, AfterSD;
    m_pSwapChain->GetDesc(&CurrentSD);
    hr= m_pSwapChain->ResizeBuffers(CurrentSD.BufferCount, width, height,
        CurrentSD.BufferDesc.Format, 0);

   //	변경된 백 버퍼의 크기를 얻고 렌더타켓 뷰를 다시 생성 및 적용.
        //	뷰포트 재 지정.
    if (FAILED(hr = CreateRenderTargetView()))
    {
        return false;
    }
    if (FAILED(hr = CreateDepthStencilView()))
    {
        return false;
    }
    CreateViewport();

    CreateDXResource();
    return S_OK;
}
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
    if (FAILED(hr = CreateDepthStencilView()))
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
    // 1)디바이스 생성
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
    /*HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), 
        (void**)&m_pGIFactory);*/
    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory),
        (void**)m_pGIFactory.GetAddressOf()); 
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
    return m_pGIFactory->CreateSwapChain(m_pd3dDevice.Get(), 
        &sd, m_pSwapChain.GetAddressOf());
}

HRESULT TDevice::CreateRenderTargetView()
{
    HRESULT hr;
    // 4)랜더타켓뷰 생성
    ID3D11Texture2D* pBackBuffer = nullptr;
    m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
    hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer,
        NULL, m_pRTV.GetAddressOf());
    pBackBuffer->Release();
    return hr;
}
HRESULT TDevice::CreateDepthStencilView()
{
    HRESULT hr;
    D3D11_RENDER_TARGET_VIEW_DESC rtvd;
    m_pRTV->GetDesc(&rtvd);
    DXGI_SWAP_CHAIN_DESC scd;
    m_pSwapChain->GetDesc(&scd);

    // 1번 텍스처를 생성한다.
    ComPtr<ID3D11Texture2D>  pDSTexture;
    D3D11_TEXTURE2D_DESC td;
    ZeroMemory(&td, sizeof(td));
    td.Width= scd.BufferDesc.Width;
    td.Height= scd.BufferDesc.Height;
    td.MipLevels=1;
    td.ArraySize=1;
    td.Format = DXGI_FORMAT_R24G8_TYPELESS;
    td.SampleDesc.Count=1;
    td.Usage = D3D11_USAGE_DEFAULT;    
    td.CPUAccessFlags=0;
    td.MiscFlags=0;
    td.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    hr = m_pd3dDevice->CreateTexture2D(&td, NULL, pDSTexture.GetAddressOf());
    // 2번 깊이스텐실 뷰로 생성한다.
    D3D11_DEPTH_STENCIL_VIEW_DESC dtvd;
    ZeroMemory(&dtvd, sizeof(dtvd));
    dtvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dtvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    hr=m_pd3dDevice->CreateDepthStencilView(pDSTexture.Get(), &dtvd, m_pDepthStencilView.GetAddressOf());
    // 3번 뷰 적용
    //m_pImmediateContext->OMSetRenderTargets(1, m_pRTV.GetAddressOf(),
       //m_pDepthStencilView.Get());
    // 4번 깊이스텐실 뷰 상태 객체 생성해서 적용
    return hr;
}
void TDevice::CreateViewport()
{
    // 5)뷰포트 설정
    m_vp.Width  = g_rtClient.right;
    m_vp.Height = g_rtClient.bottom;
    m_vp.TopLeftX = 0;
    m_vp.TopLeftY = 0;
    m_vp.MinDepth = 0.0f;
    m_vp.MaxDepth = 1.0f;
    m_pImmediateContext->RSSetViewports(1, &m_vp);
}