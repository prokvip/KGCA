#pragma once
#include "TWindow.h"
class TCore : public TWindow
{
private:
	bool	CoreInit();	
		bool	CoreFrame();
		bool	CoreRender();
	bool	CoreRelease();
public:
	bool	GameRun();
public:
	virtual bool	Init() {
		return true;
	};
	virtual bool	Frame() {
		return true;
	};
	virtual bool	Render() {
		return true;
	};
	virtual bool	Release() {
		return true;
	};
};

