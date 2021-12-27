#pragma once
#include "TWindow.h"
class TCore : public TWindow
{
public:
	bool	CoreInit();
	bool	GameRun();
		bool	CoreFrame();
		bool	CoreRender();
	bool	CoreRelease();
};

