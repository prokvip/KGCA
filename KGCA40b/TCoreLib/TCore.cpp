#include "TCore.h"
bool	TCore::CoreInit()
{
	CreateDevice();
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
	return true;
}
bool	TCore::CoreFrame()
{
	Frame();
	return true;
}
bool	TCore::CoreRender()
{	
	float color[4] = { 1.0f, 0.0f, 0.0f,1.0f };
	m_pImmediateContext->ClearRenderTargetView(
		m_pRenderTargetView,
		color);
	
	// 백버퍼에 랜더링 한다.
	Render();

	m_pSwapChain->Present(0, 0);
	return true;
}

bool	TCore::CoreRelease()
{
	Release();
	CleapupDevice();
	return true;
}