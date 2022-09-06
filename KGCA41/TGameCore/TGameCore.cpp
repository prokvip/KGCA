#include "TGameCore.h"
bool		TGameCore::TCoreInit()
{
	TDevice::Init();
	I_Input.Init();
	I_Timer.Init();
    return Init();
}
bool		TGameCore::TCoreFrame()
{
	I_Input.Frame();
	I_Timer.Frame();
    return Frame();
}
bool		TGameCore::TCorePreRender()
{
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRTV, NULL);
	float color[4] = { 1.0f,1.0f,1.0f,1.0f };
	m_pImmediateContext->ClearRenderTargetView(m_pRTV, color);
    return true;
}
bool		TGameCore::TCoreRender()
{  	
	TCorePreRender();
	Render();
	I_Input.Render();
	I_Timer.Render();
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
	I_Input.Release();
	I_Timer.Release();
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