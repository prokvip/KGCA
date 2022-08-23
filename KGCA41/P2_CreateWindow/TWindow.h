#pragma once
#include "TStd.h"
class TWindow
{
public:
	HINSTANCE   m_hInstance;	// 접수번호
	HWND		m_hWnd;			// 등기번호
public:
	bool		SetWindow(HINSTANCE hInst, const WCHAR* szTitle, UINT iWidth=1024, UINT iHeight=768);
	ATOM		MyRegisterClass();
	BOOL		InitInstance(const WCHAR* szTitle, UINT iWidth, UINT iHeight);
	bool		Run();
public:
	virtual bool		Init(); // 초기화
	virtual bool		Frame();// 실시간 계산
	virtual bool		Render();// 실시간 랜더링
	virtual bool		Release();// 소멸 및 삭제	
};

