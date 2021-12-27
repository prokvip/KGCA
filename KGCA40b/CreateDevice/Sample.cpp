#include "Sample.h"
bool	Sample::Init()
{
	//ID3D11Device*			m_pd3dDevice;	// ����̽� ��ü
	//ID3D11DeviceContext*	m_pImmediateContext;// �ٺ��̽� ���ؽ�Ʈ ��ü
	//IDXGISwapChain*			m_pSwapChain;	// ����ü�� ��ü
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
		NULL, // �׷���ī�� IDXGIAdapter * pAdapter,
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
	//ID3D11RenderTargetView* m_pRenderTargetView;// ���� ����Ÿ�� ��
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
	if(m_pd3dDevice)m_pd3dDevice->Release();	// ����̽� ��ü
	if (m_pImmediateContext)m_pImmediateContext->Release();// �ٺ��̽� ���ؽ�Ʈ ��ü
	if (m_pSwapChain)m_pSwapChain->Release();	// ����ü�� ��ü
	if (m_pRenderTargetView)m_pRenderTargetView->Release();
	m_pd3dDevice = nullptr;	// ����̽� ��ü
	m_pImmediateContext = nullptr;// �ٺ��̽� ���ؽ�Ʈ ��ü
	m_pSwapChain = nullptr;	// ����ü�� ��ü
	m_pRenderTargetView = nullptr;
	return true;
}
Sample::Sample()
{
	m_pd3dDevice = nullptr;	// ����̽� ��ü
	m_pImmediateContext = nullptr;// �ٺ��̽� ���ؽ�Ʈ ��ü
	m_pSwapChain = nullptr;	// ����ü�� ��ü
	m_pRenderTargetView = nullptr;
}



GAME_RUN(CreateDevice, 1024, 768);