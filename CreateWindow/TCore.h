#pragma once
#include "TWindow.h"
#include "TDevice.h"
#include "TTimer.h"
#include "TInput.h"
#include "TSound.h"
class TCore : public TWindow
{
	TDevice     m_Device;
	TTimer		m_Timer;
	TInput		m_Input;
	TSound		m_Sound;
private:
	bool	GameInit()	override;
	bool	GameRun()	override;
	bool	GameFrame();
	bool	GameRender();
	bool	GameRelease()override;
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
public:
};

