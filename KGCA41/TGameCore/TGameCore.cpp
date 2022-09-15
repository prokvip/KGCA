#include "TGameCore.h"
bool		TGameCore::TCoreInit()
{
	TDevice::Init();
	I_Tex.SetDevice(m_pd3dDevice, m_pImmediateContext);
	I_Shader.SetDevice(m_pd3dDevice, m_pImmediateContext);
	I_Sound.Init();
	I_Input.Init();
	I_Timer.Init();
	m_Writer.Init();
	IDXGISurface1* pBackBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface1),
		(void**)&pBackBuffer);
	m_Writer.Set(pBackBuffer);
	pBackBuffer->Release();

	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;// 최근점 필터링
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	HRESULT hr = m_pd3dDevice->CreateSamplerState(&sd, &m_pDefaultSS);
    return Init();
}
bool		TGameCore::TCoreFrame()
{
	I_Input.Frame();
	I_Timer.Frame();
	I_Sound.Frame();
	m_Writer.Frame();
    return Frame();
}
bool		TGameCore::TCorePreRender()
{
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRTV, NULL);
	float color[4] = { 0.34324f,0.52342f,0.798320f,1.0f };
	m_pImmediateContext->ClearRenderTargetView(m_pRTV, color);
	m_pImmediateContext->PSSetSamplers(0, 1, &m_pDefaultSS);
	m_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    return true;
}
bool		TGameCore::TCoreRender()
{  	
	TCorePreRender();
		Render();
		I_Input.Render();
		I_Timer.Render();
		m_Writer.m_szDefaultText = I_Timer.m_szTimer;
		m_Writer.Render();
	TCorePostRender();
    return true;
}
bool		TGameCore::TCorePostRender()
{
	m_pSwapChain->Present(0, 0);
    return true;
}
bool		TGameCore::TCoreRelease()
{   
	Release();
	if (m_pDefaultSS)m_pDefaultSS->Release();
	I_Input.Release();
	I_Timer.Release();
	m_Writer.Release();
	I_Sound.Release();
	TDevice::Release();
    return true;
}
bool        TGameCore::Run()
{    
	TCoreInit();	
	while (m_bGameRun)
	{
		if (TWindow::Run() == true)
		{
			TCoreFrame();
			TCoreRender();
		}
		else
		{
			m_bGameRun = false;
		}
	}
	TCoreRelease();
	return true;
}