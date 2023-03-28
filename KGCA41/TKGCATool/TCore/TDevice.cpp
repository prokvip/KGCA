#include "pch.h"
#include "TDevice.h"

FrameBuffer TDevice::createFrameBuffer(UINT width, UINT height, UINT samples, DXGI_FORMAT colorFormat, DXGI_FORMAT depthstencilFormat) const
{
    FrameBuffer fb;
    fb.width = width;
    fb.height = height;
    fb.samples = samples;

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.SampleDesc.Count = samples;

    if (colorFormat != DXGI_FORMAT_UNKNOWN) {
        desc.Format = colorFormat;
        desc.BindFlags = D3D11_BIND_RENDER_TARGET;
        if (samples <= 1) {
            desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
        }
        if (FAILED(m_pd3dDevice->CreateTexture2D(&desc, nullptr, &fb.colorTexture))) {
            throw std::runtime_error("Failed to create FrameBuffer color texture");
        }

        D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
        rtvDesc.Format = desc.Format;
        rtvDesc.ViewDimension = (samples > 1) ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;
        if (FAILED(m_pd3dDevice->CreateRenderTargetView(fb.colorTexture.Get(), &rtvDesc, &fb.rtv))) {
            throw std::runtime_error("Failed to create FrameBuffer render target view");
        }

        if (samples <= 1) {
            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
            srvDesc.Format = desc.Format;
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MostDetailedMip = 0;
            srvDesc.Texture2D.MipLevels = 1;
            if (FAILED(m_pd3dDevice->CreateShaderResourceView(fb.colorTexture.Get(), &srvDesc, &fb.srv))) {
                throw std::runtime_error("Failed to create FrameBuffer shader resource view");
            }
        }
    }

    if (depthstencilFormat != DXGI_FORMAT_UNKNOWN) {
        desc.Format = depthstencilFormat;
        desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        if (FAILED(m_pd3dDevice->CreateTexture2D(&desc, nullptr, &fb.depthStencilTexture))) {
            throw std::runtime_error("Failed to create FrameBuffer depth-stencil texture");
        }

        D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
        dsvDesc.Format = desc.Format;
        dsvDesc.ViewDimension = (samples > 1) ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
        if (FAILED(m_pd3dDevice->CreateDepthStencilView(fb.depthStencilTexture.Get(), &dsvDesc, &fb.dsv))) {
            throw std::runtime_error("Failed to create FrameBuffer depth-stencil view");
        }
    }

    return fb;
}

void TDevice::resolveFrameBuffer(const FrameBuffer& srcfb, const FrameBuffer& dstfb, DXGI_FORMAT format) const
{
    if (srcfb.colorTexture != dstfb.colorTexture) {
        m_pImmediateContext->ResolveSubresource(dstfb.colorTexture.Get(), 0, srcfb.colorTexture.Get(), 0, format);
    }
}
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
    UINT pNumQualityLevels;
    HRESULT hr = m_pd3dDevice->CheckMultisampleQualityLevels(
        DXGI_FORMAT_R8G8B8A8_UNORM,
        4,//D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT,
        &pNumQualityLevels
    );

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

    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    hr = m_pGIFactory->CreateSwapChain(m_pd3dDevice.Get(), &sd, m_pSwapChain.GetAddressOf());

    // Determine maximum supported MSAA level.
    UINT samples;
    UINT colorQualityLevels;
    UINT depthStencilQualityLevels;
    for (samples = 16; samples > 1; samples /= 2) 
    {        
        m_pd3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R16G16B16A16_FLOAT, samples, &colorQualityLevels);
        m_pd3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_D24_UNORM_S8_UINT, samples, &depthStencilQualityLevels);
        if (colorQualityLevels > 0 && depthStencilQualityLevels > 0) {
            break;
        }
    }

    m_framebuffer = createFrameBuffer(g_rtClient.right, g_rtClient.bottom, samples, DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_D24_UNORM_S8_UINT);
    if (samples > 1) {
        m_resolveFramebuffer = createFrameBuffer(g_rtClient.right, g_rtClient.bottom, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, (DXGI_FORMAT)0);
    }
    else {
        m_resolveFramebuffer = m_framebuffer;
    }
    return hr;
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