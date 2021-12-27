#include "Sample.h"
bool	Sample::Init()
{
	//ID3D11Device*			m_pd3dDevice;	// 디바이스 객체
	//ID3D11DeviceContext*	m_pImmediateContext;// 다비이스 컨텍스트 객체
	//IDXGISwapChain*			m_pSwapChain;	// 스왑체인 객체
	UINT Flags = 0;
	D3D_FEATURE_LEVEL fl[]
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0,
	};
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferDesc.Width = m_rtClient.right;
	scd.BufferDesc.Height= m_rtClient.bottom;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.SampleDesc.Count = 1;
	scd.BufferUsage=DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = m_hWnd;
	scd.Windowed = true;
	

	D3D_FEATURE_LEVEL pFeatureLevel;
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL, // 그래픽카드 IDXGIAdapter * pAdapter,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		Flags,
		fl,
		1,
		D3D11_SDK_VERSION,
		&scd,
		&m_pSwapChain,
		&m_pd3dDevice,
		&pFeatureLevel,
		&m_pImmediateContext);	
	if ( FAILED(hr))
	{
		return false;
	}
	//ID3D11RenderTargetView* m_pRenderTargetView;// 메인 랜더타켓 뷰
	return true;
}
bool	Sample::Frame()
{
	return true;
}
bool	Sample::Render()
{
	return true;
}
bool	Sample::Release()
{
	if(m_pd3dDevice)m_pd3dDevice->Release();	// 디바이스 객체
	if (m_pImmediateContext)m_pImmediateContext->Release();// 다비이스 컨텍스트 객체
	if (m_pSwapChain)m_pSwapChain->Release();	// 스왑체인 객체
	if (m_pRenderTargetView)m_pRenderTargetView->Release();
	m_pd3dDevice = nullptr;	// 디바이스 객체
	m_pImmediateContext = nullptr;// 다비이스 컨텍스트 객체
	m_pSwapChain = nullptr;	// 스왑체인 객체
	m_pRenderTargetView = nullptr;
	return true;
}
Sample::Sample()
{
	m_pd3dDevice = nullptr;	// 디바이스 객체
	m_pImmediateContext = nullptr;// 다비이스 컨텍스트 객체
	m_pSwapChain = nullptr;	// 스왑체인 객체
	m_pRenderTargetView = nullptr;
}



GAME_RUN(CreateDevice, 1024, 768);