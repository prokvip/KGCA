#include "TCore.h"
bool	TCore::CoreInit()
{
	m_GameTimer.Init();
	TInput::Get().Init();
	InitDeivice();
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
	m_GameTimer.Frame();
	TInput::Get().Frame();
	Frame();
	return true;
}
bool	TCore::CoreRender()
{	
	//float color[4] = { 0.1543f, 0.23421f, 0.4323f,1.0f };
	float color[4] = { 1, 0, 0,1.0f };
	m_pImmediateContext->ClearRenderTargetView(
		m_pRenderTargetView,
		color);
	
	// 백버퍼에 랜더링 한다.
	Render();

	m_GameTimer.Render();
	TInput::Get().Render();
	m_pSwapChain->Present(0, 0);
	return true;
}

bool	TCore::CoreRelease()
{
	Release();

	m_GameTimer.Release();
	TInput::Get().Release();
	CleapupDevice();
	return true;
}

TCore::TCore()
{

}
TCore::~TCore()
{

}