#pragma once
#include <windows.h>
class TWindow
{
public:
	HINSTANCE   m_hInstance;	// 접수번호
	HWND		m_hWnd;			// 등기번호
public:
	bool		Init(); // 초기화
	bool		Frame();// 실시간 계산
	bool		Render();// 실시간 랜더링
	bool		Release();// 소멸 및 삭제
	bool		Run();
public:
	ATOM		MyRegisterClass(HINSTANCE hInstance);
	BOOL		InitInstance(HINSTANCE hInstance);
};

