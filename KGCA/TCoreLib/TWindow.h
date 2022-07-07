#pragma once
#include "TStd.h"
class TWindow 
{
public:
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;
	RECT		m_rtClient;
	RECT		m_rtWindow;
public:
	BOOL  SetWinClass(HINSTANCE hInstance);
	BOOL  SetWindow(const WCHAR* szTitle=L"Sample", 
					int iWidth = 800, 
					int iHeight= 600);
	bool  WinRun();
	virtual LRESULT  MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void     ResizeDevice(UINT iWidth, UINT iHeight);
public:
	TWindow();
	virtual ~TWindow();
};

