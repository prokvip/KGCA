#include "TCore.h"
#include "TObjectMgr.h"
#include "TSoundMgr.h"
bool	TCore::CoreInit()
{
	m_GameTimer.Init();	
	TInput::Get().Init();
	if( SUCCEEDED(InitDeivice()))
	{
		I_Shader.Set(m_pd3dDevice.Get());
		I_Texture.Set(m_pd3dDevice.Get());
		TDxState::SetState(m_pd3dDevice.Get());
		if (m_dxWrite.Init())
		{
			IDXGISurface1* pSurface = nullptr;
			HRESULT hr = m_pSwapChain->GetBuffer(0, 
				__uuidof(IDXGISurface1),
				(void**)&pSurface);
			if (SUCCEEDED(hr))
			{
				m_dxWrite.SetRenderTarget(pSurface);
			}
			if (pSurface) pSurface->Release();
		}
	}
	Init();

	return true;
}
bool	TCore::GameRun()
{
	CoreInit();
	while(WinRun())
	{
		CoreFrame();
		CoreRender();
	}
	CoreRelease();

	MemoryReporting();
	return true;
}
bool	TCore::CoreFrame()
{
	m_GameTimer.Frame();
	TInput::Get().Frame();
	I_ObjectMgr.Frame();
	I_Sound.Frame();
	Frame();
	m_dxWrite.Frame();
	return true;
}
bool	TCore::CoreRender()
{	
	float color[4] = { 0.1543f, 0.23421f, 0.4323f,1.0f };
	//float color[4] = { 1, 0, 0,1.0f };
	m_pImmediateContext->ClearRenderTargetView(
		m_pRenderTargetView.Get(),
		color);
	m_pImmediateContext->ClearDepthStencilView(
		m_pDepthStencilView.Get(), 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_pImmediateContext->OMSetRenderTargets(1, 
		m_pRenderTargetView.GetAddressOf(),
		m_pDepthStencilView.Get());

	m_pImmediateContext->PSSetSamplers(0, 1, &TDxState::m_pSamplerState);	
	m_pImmediateContext->OMSetDepthStencilState(TDxState::g_pDSSDepthEnable, 0x00);
	m_pImmediateContext->RSSetState(TDxState::g_pRSBackCullSolid);

	// ����ۿ� ������ �Ѵ�.
	Render();

	m_GameTimer.Render();
	TInput::Get().Render();

	m_dxWrite.Render();
	m_pSwapChain->Present(0, 0);
	return true;
}

bool	TCore::CoreRelease()
{
	Release();
	TDxState::Release();
	m_dxWrite.Release();
	m_GameTimer.Release();
	TInput::Get().Release();
	CleapupDevice();	
	return true;
}
void     TCore::ResizeDevice(UINT iWidth, UINT iHeight)
{
	if (m_pd3dDevice == nullptr) return;
	DeleteResizeDevice(iWidth, iHeight);

	m_dxWrite.DeleteDeviceResize();
		
		TDevice::ResizeDevice(iWidth, iHeight);
	
	IDXGISurface1* pSurface = nullptr;
	HRESULT hr = m_pSwapChain->GetBuffer(0,
		__uuidof(IDXGISurface1),
		(void**)&pSurface);
	if (SUCCEEDED(hr))
	{
		m_dxWrite.SetRenderTarget(pSurface);
	}
	if (pSurface) pSurface->Release();

	CreateResizeDevice(iWidth, iHeight);
}
TCore::TCore()
{

}
TCore::~TCore()
{
	
}