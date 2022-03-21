#pragma once
#include "TDevice.h"
#include "TDxObject.h"
#include "TInput.h"
#include "TTimer.h"
#include "TWriteFont.h"
class TCore : public TDevice
{
public:
	TTimer		m_GameTimer;
	TWriteFont  m_dxWrite;
	bool		m_bWireFrame = false;
private:
	bool	CoreInit();	
		bool	CoreFrame();
		bool	CoreRender();
	bool	CoreRelease();
public:
	bool	GameRun();
public:
	virtual void	CreateResizeDevice(UINT iWidth, UINT iHeight) {};
	virtual void	DeleteResizeDevice(UINT iWidth, UINT iHeight) {};
	void			ResizeDevice(UINT iWidth, UINT iHeight);
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

