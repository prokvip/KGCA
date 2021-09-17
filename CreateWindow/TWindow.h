#pragma once
#include <windows.h>
class TWindow
{
public:
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;
	bool		m_bGameRun;
public:
	bool   InitWindows(HINSTANCE hInstance,
		int nCmdShow,
		const WCHAR* strWindowTitle);	
	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	bool	Run();
	bool	GameInit();
	bool	GameRun();
		bool	GameFrame();
		bool	GameRender();
	bool	GameRelease();
public:
	TWindow();
};

