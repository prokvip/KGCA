#include "TCore.h"
#include "TObjectMgr.h"
#include "TSoundMgr.h"
bool	TCore::CoreInit()
{
	m_GameTimer.Init();	
	TInput::Get().Init();
	if( SUCCEEDED(InitDeivice()))
	{
		I_Shader.Set(m_pd3dDevice);
		I_Texture.Set(m_pd3dDevice);

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

	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
	sd.Filter= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.MinLOD = FLT_MAX;
	sd.MaxLOD = FLT_MIN;
	HRESULT hr = m_pd3dDevice->CreateSamplerState(&sd ,&m_pSamplerState);
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
	//float color[4] = { 0.1543f, 0.23421f, 0.4323f,1.0f };
	float color[4] = { 1, 0, 0,1.0f };
	m_pImmediateContext->ClearRenderTargetView(
		m_pRenderTargetView,
		color);
	m_pImmediateContext->PSSetSamplers(0, 1, &m_pSamplerState);

	// 백버퍼에 랜더링 한다.
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
	if (m_pSamplerState)m_pSamplerState->Release();

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