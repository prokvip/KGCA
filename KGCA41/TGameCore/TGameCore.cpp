#include "TGameCore.h"
bool		TGameCore::TCoreInit()
{
	TDevice::Init();
    return Init();
}
bool		TGameCore::TCoreFrame()
{
	TDevice::Frame();
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