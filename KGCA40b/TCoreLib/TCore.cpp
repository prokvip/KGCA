#include "TCore.h"
bool	TCore::CoreInit()
{
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
	Render();
	return true;
}
bool	TCore::CoreRelease()
{
	Release();
	return true;
}