#pragma once
#include "TDevice.h"

class TCore : public TDevice
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
public:
	TCore();
	virtual ~TCore();
};

