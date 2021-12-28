#include "TDevice.h"
HRESULT TDevice::InitDeivice()
{
	HRESULT hr = S_OK; 
	CreateDevice();
	CreateRenderTargetView();
	SetViewport();
	return hr;
}
bool	TDevice::CreateDevice()
{
	UINT Flags = 0;
	D3D_FEATURE_LEVEL fl[]
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0,
	};

	ZeroMemory(&m_SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	m_SwapChainDesc.BufferDesc.Width = m_rtClient.right;
	m_SwapChainDesc.BufferDesc.Height = m_rtClient.bottom;
	m_SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	m_SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	m_SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_SwapChainDesc.SampleDesc.Count = 1;
	m_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	m_SwapChainDesc.BufferCount = 1;
	m_SwapChainDesc.OutputWindow = m_hWnd;
	m_SwapChainDesc.Windowed = true;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		Flags,
		fl,
		1,
		D3D11_SDK_VERSION,
		&m_SwapChainDesc,
		&m_pSwapChain,
		&m_pd3dDevice,
		&m_FeatureLevel,
		&m_pImmediateContext);
	if (FAILED(hr))
	{
		return false;
	}	
	return true;
}
bool	TDevice::CreateRenderTargetView()
{
	ID3D11Texture2D* backBuffer = nullptr;
	m_pSwapChain->GetBuffer(0,
		__uuidof(ID3D11Texture2D),
		(LPVOID*)&backBuffer);
	m_pd3dDevice->CreateRenderTargetView(
		backBuffer,
		NULL,
		&m_pRenderTargetView);
	if (backBuffer)backBuffer->Release();

	m_pImmediateContext->OMSetRenderTargets(
		1,
		&m_pRenderTargetView, NULL);	
	return true;
}
bool	TDevice::SetViewport()
{	
	// 뷰포트 세팅
	//DXGI_SWAP_CHAIN_DESC swapDesc;
	//m_pSwapChain->GetDesc(&swapDesc);

	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_ViewPort.Width = m_SwapChainDesc.BufferDesc.Width;
	m_ViewPort.Height = m_SwapChainDesc.BufferDesc.Height;
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
	m_pImmediateContext->RSSetViewports(1, &m_ViewPort);
	return true;
}
bool	TDevice::CleapupDevice()
{
	if (m_pd3dDevice)m_pd3dDevice->Release();	// 디바이스 객체
	if (m_pImmediateContext)m_pImmediateContext->Release();// 다비이스 컨텍스트 객체
	if (m_pSwapChain)m_pSwapChain->Release();	// 스왑체인 객체
	if (m_pRenderTargetView)m_pRenderTargetView->Release();
	m_pd3dDevice = nullptr;	// 디바이스 객체
	m_pImmediateContext = nullptr;// 다비이스 컨텍스트 객체
	m_pSwapChain = nullptr;	// 스왑체인 객체
	m_pRenderTargetView = nullptr;
	return true;
}
TDevice::TDevice()
{
	m_pd3dDevice = nullptr;	// 디바이스 객체
	m_pImmediateContext = nullptr;// 다비이스 컨텍스트 객체
	m_pSwapChain = nullptr;	// 스왑체인 객체
	m_pRenderTargetView = nullptr;
}
TDevice::~TDevice()
{}