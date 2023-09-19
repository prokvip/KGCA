#pragma once
#include "TStd.h"
class TWindow
{
public:
    HINSTANCE   m_hInstance;
    HWND        m_hWnd;
    DWORD       m_dwExStyle = 0;
    DWORD       m_dwStyle = WS_OVERLAPPEDWINDOW;
    DWORD       m_dwWindowPosX = 0;
    DWORD       m_dwWindowPosY = 0;
    DWORD       m_dwClientWidth;
    DWORD       m_dwClientHeight;
    RECT        m_rcClient;
public:
    virtual int MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    virtual void ResizeDevice(UINT width, UINT height) {};
    bool  SetRegisterClassWindow(HINSTANCE hInstance);
    bool  SetWindow(const WCHAR* szTitle, //std::wstring szTitle, 
                    DWORD       dwWindowWidth =800,
                    DWORD       dwWindowHeight=600);
public:
    TWindow();
    virtual ~TWindow() {}
public:
};

