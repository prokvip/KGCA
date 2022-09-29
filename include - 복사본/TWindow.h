#pragma once
#include "TStd.h"
class TWindow 
{
public:
	HINSTANCE   m_hInstance;	// 접수번호
	HWND		m_hWnd;			// 등기번호
	RECT		m_rtWindow;
	RECT		m_rtClient;
	DWORD       m_csStyle;
	UINT		m_iClientWidth;
	UINT		m_iClientHeight;
public:
	bool		SetWindow(HINSTANCE hInst, const WCHAR* szTitle, UINT iWidth=1024, UINT iHeight=768);
	ATOM		MyRegisterClass();
	BOOL		InitInstance(const WCHAR* szTitle, UINT iWidth, UINT iHeight);
	virtual bool		Run();	
	void		CenterWindow();
	virtual LRESULT		MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
	TWindow();
};

