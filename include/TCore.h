#pragma once
#include "TWindow.h"
#include "TDevice.h"
#include "TTimer.h"
#include "TInput.h"
#include "TSound.h"
#include "TWrite.h"
#include "TCamera.h"
#include "TDxState.h"
class TCore : public TWindow
{
public:
	TTimer				m_Timer;
	TSound				m_Sound;
	TWrite				m_Write;
	TDebugCamera		m_Camera;
	bool		m_bDebugText = false;
private:
	bool	GameInit()	override;
	bool	GameRun()	override;
	bool	GameFrame();
	bool	GameRender();
	bool	GameRelease()override;
public:
	void	FrameCamera();
	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)override;
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	PreRender();
	virtual bool	Render();
	virtual bool	PostRender();
	virtual bool	Release();
public:
};

