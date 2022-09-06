#include "Sample.h"
bool Sample::Init()
{
	TDevice::Init();
	return true;
}
bool Sample::Frame()
{
	TDevice::Frame();
	return true;
}
bool Sample::Render()
{
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRTV, NULL);
	float color[4] = { 1.0f,1.0f,1.0f,1.0f };
	m_pImmediateContext->ClearRenderTargetView(m_pRTV, color);
	m_pSwapChain->Present(0, 0);
	return true;
}
bool Sample::Release()
{
	TDevice::Release();
	return true;
}
bool Sample::Run()
{
	Init();
	bool bGameRun = true;
	while (bGameRun)
	{
		if (TWindow::Run() == true)
		{
			Frame();
			Render();			
		}
		else
		{
			bGameRun = false;
		}		
	}
	
	Release();
	return true;
}
GAME_RUN(TGameCoreSample1, 800,600)