#pragma once
#include "TWindow.h"
#include "TDevice.h"
#include "TTimer.h"
#include "TInput.h"
#include "TSound.h"
#include "TWrite.h"
class TCore : public TWindow
{
public:
	TTimer		m_Timer;
	//TInput		m_Input;
	TSound		m_Sound;
	TWrite		m_Write;
	bool		m_bDebugText = false;
private:
	bool	GameInit()	override;
	bool	GameRun()	override;
	bool	GameFrame();
	bool	GameRender();
	bool	GameRelease()override;
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	PreRender();
	virtual bool	Render();
	virtual bool	PostRender();
	virtual bool	Release();
public:
};

