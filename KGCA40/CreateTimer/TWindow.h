#pragma once
#include <windows.h>
#include "TDevice.h"
class TWindow : public TDevice
{
public:
	RECT m_rtWindow;
	RECT m_rtClient;

	HINSTANCE	m_hInstance;
	HWND		m_hWnd;
	bool		m_bGameRun;
public:
	bool   InitWindows(HINSTANCE hInstance,
		int nCmdShow,
		const WCHAR* strWindowTitle);	
	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	bool	Run();
	virtual bool	GameInit();
	virtual bool	GameRun();
	virtual bool	GameRelease();
public:
	TWindow();
};

