#pragma once
#include "TDevice.h"

class TWindow : public TDevice
{
public:
	//--------------------------------------------------------------------------------------
	// Global Variables
	//--------------------------------------------------------------------------------------
	HINSTANCE               m_hInstance;		// 인스턴스 핸들
	HWND                    m_hWnd;				// 윈도우 핸들
	DWORD					m_dwWindowStyle;	// 윈도우 스타일
	RECT					m_rcWindowBounds;   // 윈도우 영역
	RECT					m_rcWindowClient;   // 클라이언트 영역	
	UINT					m_iWindowWidth;		// 클라이언트 영역 가로크기
	UINT					m_iWindowHeight;	// 을라이언트 영역 세로크기
public:
	// 윈도우 생성
	bool				InitWindow(		HINSTANCE hInstance, 
										int nCmdShow, 
										const TCHAR* strWindowTitle = _T("TBasis Sample"), 										
										int iWidth = 800,
										int iHeight=600,
										BOOL IsFullScreen = FALSE);	
	// 윈도우 위치 화면 중앙으로 이동
	void				CenterWindow(HWND hwnd);
	bool				CheckWindowSize();
	
public:
	virtual int			WndProc( HWND, UINT, WPARAM, LPARAM );
			LRESULT		WindowMsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
public:
	TWindow(void);
	virtual ~TWindow(void);
};