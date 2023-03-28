#include "TDevice.h"
namespace TBASIS
{
	ID3D11Device*			g_pd3dDevice = NULL;
	ID3D11DeviceContext*    g_pImmediateContext = NULL;
	UINT					g_iClientWidth;
	UINT					g_iClientHeight;
}
HRESULT TDevice::InitDevice( HWND hWnd, UINT iWidth, UINT iHeight, BOOL IsFullScreen )
{	
	HRESULT hr = S_OK;	
	if( FAILED( hr = CreateDevice() ))
	{
		MessageBox( 0, _T("CreateDevice  ����"), _T("Fatal error"), MB_OK );
		return hr;
	}	
	if( FAILED( hr = CreateSwapChain(hWnd,iWidth, iHeight, IsFullScreen) ) )
	{
		MessageBox( 0, _T("CreateSwapChain  ����"), _T("Fatal error"), MB_OK );
		return hr;
	}	
	if( FAILED( hr = SetRenderTargetView() ) )
	{
		MessageBox( 0, _T("SetRenderTargetView  ����"), _T("Fatal error"), MB_OK );
		return hr;
	}
	if( FAILED( hr = SetViewPort() ) )
	{
		MessageBox( 0, _T("SetViewPort  ����"), _T("Fatal error"), MB_OK );
		return hr;
	}	
	// Alt + Enter Ű�� ���´�.
	if( FAILED( hr = GetGIFactory()->MakeWindowAssociation( hWnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER  )))
	{
		return hr;
	}
	return hr;
}

BOOL TDevice::GetFullScreenFlag()
{
	return m_IsFullScreenMode;
}
void TDevice::SetFullScreenFlag( BOOL bFlag )
{
	m_IsFullScreenMode = bFlag;
}
HRESULT TDevice::ResizeDevice( UINT iWidth, UINT iHeight)
{
	if( m_pd3dDevice == NULL ) return true;
	HRESULT hr;
	// ���ε��Ǿ� �ִ� ���� �丮�ҽ��� �����Ѵ�.
	if( FAILED( hr= DeleteDxResource()))
	{
		return hr;
	}
	//--------------------------------------------------------------------------------------
	// ����Ÿ�ϰ� ���̽��ٽ� ���۸� �����Ѵ�.
	//--------------------------------------------------------------------------------------
	m_pImmediateContext->OMSetRenderTargets( 0, NULL, NULL );
	m_DefaultRT.m_pRenderTargetView.ReleaseAndGetAddressOf();	

	//--------------------------------------------------------------------------------------
	// ������� ũ�⸦ �����Ѵ�.
	//--------------------------------------------------------------------------------------
	if (FAILED(hr = m_pSwapChain->ResizeBuffers(m_SwapChainDesc.BufferCount, iWidth, iHeight, m_SwapChainDesc.BufferDesc.Format, m_SwapChainDesc.Flags)))
	{
		return hr;
	}
	if (FAILED(hr = m_pSwapChain->GetDesc(&m_SwapChainDesc)))
	{
		return hr;
	}
	TBASIS::g_iClientWidth = m_SwapChainDesc.BufferDesc.Width;
	TBASIS::g_iClientHeight = m_SwapChainDesc.BufferDesc.Height;

	// ����Ÿ�Ϻ� ���� �� �����Ѵ�.
    if( FAILED( hr = SetRenderTargetView() ) ) 
	{
		return hr;
	}
	// ����Ʈ�� �����ϰ� �����Ѵ�.
	if( FAILED( hr = SetViewPort() ) )
	{
		return hr;
	}

	if (FAILED(hr = CreateDxResource()))
	{
		return hr;
	}
	return S_OK;
}
ID3D11Device* TDevice::GetDevice()
{
	assert( m_pd3dDevice );
	return m_pd3dDevice; 
}
ID3D11DeviceContext* TDevice::GetContext()
{
	assert( m_pImmediateContext );
	return m_pImmediateContext; 
}
IDXGISwapChain* TDevice::GetSwapChain()
{ 
	assert( m_pSwapChain );
	return m_pSwapChain; 
}
ID3D11RenderTargetView* TDevice::GetRenderTargetView()	
{ 
	return m_DefaultRT.m_pRenderTargetView.Get();
}
IDXGIFactory* TDevice::GetGIFactory()
{
	assert(m_pGIFactory);
	return m_pGIFactory;
}

//--------------------------------------------------------------------------------------
// DXGIFactory �������̽��κ��� IDXGISwapChain �������̽��� �����Ѵ�.
//--------------------------------------------------------------------------------------
HRESULT TDevice::CreateSwapChain( HWND hWnd, UINT iWidth, UINT iHeight, BOOL IsFullScreen)
{	
	HRESULT hr = S_OK;
	SetFullScreenFlag( IsFullScreen );
	if( m_pGIFactory == NULL ) return S_FALSE;
	DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width		= iWidth;
    sd.BufferDesc.Height	= iHeight;
    sd.BufferDesc.Format	= DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
     sd.Windowed = !m_IsFullScreenMode;
	// �߰�
	sd.Flags	= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	if( FAILED( hr = m_pGIFactory->CreateSwapChain( m_pd3dDevice, &sd, &m_pSwapChain ) ))
	{
		return hr;
	}	
	if (FAILED(hr = m_pSwapChain->GetDesc(&m_SwapChainDesc)))
	{
		return hr;
	}
	TBASIS::g_iClientWidth = m_SwapChainDesc.BufferDesc.Width;
	TBASIS::g_iClientHeight = m_SwapChainDesc.BufferDesc.Height;
	return hr;
}
//--------------------------------------------------------------------------------------
// DXGIFactory �������̽��κ��� IDXGISwapChain �������̽��� �����Ѵ�.
//--------------------------------------------------------------------------------------
HRESULT TDevice::CreateDeviceAndSwapChain(HWND hWnd, UINT iWidth, UINT iHeight, BOOL IsFullScreen)
{	
	HRESULT hr = S_OK;

    UINT createDeviceFlags =  D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = sizeof( driverTypes ) / sizeof( driverTypes[0] );

	D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount				= 1;
    sd.BufferDesc.Width			= iWidth;
    sd.BufferDesc.Height		= iHeight;
    sd.BufferDesc.Format		= DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage				= DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow				= hWnd;
    sd.SampleDesc.Count			= 1;
    sd.SampleDesc.Quality		= 0;
    sd.Windowed = !m_IsFullScreenMode;
	// �߰�
	sd.Flags	= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        m_driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain( NULL, m_driverType, NULL, createDeviceFlags,featureLevels, numFeatureLevels,
                                            D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, &m_FeatureLevel, &m_pImmediateContext );
        if( SUCCEEDED( hr ) )
		{
            if( FAILED( hr ) || m_FeatureLevel < D3D_FEATURE_LEVEL_11_0)
			{
				if( m_pImmediateContext ) m_pImmediateContext->Release();
				if( m_pd3dDevice ) m_pd3dDevice->Release();
				continue;
			}
			break;
		}
    }
    if( FAILED( hr ) )
	{
        return hr;
	}
	if (FAILED(hr = m_pSwapChain->GetDesc(&m_SwapChainDesc)))
	{
		return hr;
	}
	TBASIS::g_pd3dDevice = m_pd3dDevice;
	TBASIS::g_pImmediateContext = m_pImmediateContext;
	TBASIS::g_iClientWidth = m_SwapChainDesc.BufferDesc.Width;
	TBASIS::g_iClientHeight = m_SwapChainDesc.BufferDesc.Height;
	return hr;
}

//--------------------------------------------------------------------------------------
// ID3D11Device �������̽��� �����Ѵ�.
//--------------------------------------------------------------------------------------
HRESULT TDevice::CreateDevice()
{
	HRESULT hr;
	  UINT createDeviceFlags =  D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	// win10->����->�ý���/�� �� ���/������ ��� ����/��� �߰�/�׷��� ����(DirectX Graphics Tools���� �߰�) �߰� �����ϸ� �ȴ�.
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = sizeof( driverTypes ) / sizeof( driverTypes[0] );

	D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

	for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        m_driverType = driverTypes[driverTypeIndex];
        if( SUCCEEDED( hr = D3D11CreateDevice(	NULL, m_driverType, NULL, createDeviceFlags,
												featureLevels, numFeatureLevels,
												D3D11_SDK_VERSION, &m_pd3dDevice, &m_FeatureLevel, &m_pImmediateContext ) ))
		{			
			if( FAILED( hr ) || m_FeatureLevel < D3D_FEATURE_LEVEL_11_0)
			{
				if( m_pImmediateContext ) m_pImmediateContext->Release();
				if( m_pd3dDevice ) m_pd3dDevice->Release();
				continue;
			}
			break;
		}
    }
    if( FAILED( hr ) )       return hr;

	TBASIS::g_pd3dDevice = m_pd3dDevice;
	TBASIS::g_pImmediateContext = m_pImmediateContext;

	if (FAILED( hr=TDxState::SetState(m_pd3dDevice))) return hr;

	return CreateGIFactory();
}
//--------------------------------------------------------------------------------------
// DXGIFactory �������̽��� �����Ѵ�.
//--------------------------------------------------------------------------------------
HRESULT TDevice::CreateGIFactory()
{	
	if(m_pd3dDevice==NULL) return E_FAIL;
	HRESULT hr;// = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&m_pGIFactory) );
	IDXGIDevice * pDXGIDevice;
	hr = m_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);
      
	IDXGIAdapter * pDXGIAdapter;
	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter);

	pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&m_pGIFactory);
	
	pDXGIDevice->Release();
	pDXGIAdapter->Release();
	return S_OK;
}
//--------------------------------------------------------------------------------------
// DXGIFactory �������̽��κ��� IDXGISwapChain �������̽��� �����Ѵ�.
//--------------------------------------------------------------------------------------

HRESULT TDevice::SetRenderTargetView()
{
	HRESULT hr = S_OK;
	 // Create a render target view
    ID3D11Texture2D* pBackBuffer;
    hr = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
    if( FAILED( hr ) )
        return hr;

    hr = m_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, m_DefaultRT.m_pRenderTargetView.GetAddressOf() );
    pBackBuffer->Release();
    if( FAILED( hr ) )
        return hr;    
	return S_OK;
}

HRESULT TDevice::SetViewPort()
{
	HRESULT hr = S_OK;
	m_DefaultRT.m_vp.Width	= (FLOAT)m_SwapChainDesc.BufferDesc.Width;
	m_DefaultRT.m_vp.Height	= (FLOAT)m_SwapChainDesc.BufferDesc.Height;
	m_DefaultRT.m_vp.MinDepth = 0.0f;
	m_DefaultRT.m_vp.MaxDepth = 1.0f;
	m_DefaultRT.m_vp.TopLeftX = 0;
	m_DefaultRT.m_vp.TopLeftY = 0;
	return S_OK;
}

bool TDevice::CleanupDevice()
{
	ClearD3D11DeviceContext(m_pImmediateContext);
	// ���ε��Ǿ� �ִ� ���� �丮�ҽ��� �����Ѵ�.
	if( FAILED( DeleteDxResource()))
	{
		return false;
	}
       // Clear state and flush
	// ComPtr�� Attach ���Ŀ��� Release�ϸ� �ȵȴ�.
    if( m_pImmediateContext )    m_pImmediateContext->ClearState();
    if( m_pImmediateContext )    m_pImmediateContext->Flush();  
    if( m_pSwapChain ) m_pSwapChain->Release();
	if( m_pImmediateContext ) m_pImmediateContext->Release();
    if( m_pd3dDevice ) m_pd3dDevice->Release();
	if( m_pGIFactory ) m_pGIFactory->Release();
	m_pd3dDevice		= NULL;
	m_pSwapChain		= NULL;
	m_pImmediateContext = NULL;
	m_pGIFactory		= NULL;
	return true;
}
HRESULT TDevice::CreateDxResource()
{
	return S_OK;
}
HRESULT TDevice::DeleteDxResource()
{
	return S_OK;
}
TDevice::TDevice(void)
{
	m_driverType		= D3D_DRIVER_TYPE_NULL;
	m_FeatureLevel		= D3D_FEATURE_LEVEL_11_0;
	m_pd3dDevice		= NULL;
	m_pSwapChain		= NULL;
	m_pImmediateContext = NULL;
}

TDevice::~TDevice(void)
{
}
