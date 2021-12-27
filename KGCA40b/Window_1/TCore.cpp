#include "TCore.h"
bool	TCore::CoreInit()
{
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
	return true;
}
bool	TCore::CoreRender()
{
	return true;
}
bool	TCore::CoreRelease()
{
	return true;
}