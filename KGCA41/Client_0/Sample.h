#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "TGameCore.h"

class Sample : public TGameCore
{
	HWND		m_hEdit;
	HWND		m_hListbox;
	HWND		m_hSendBtn;
public:
	void		Print(const WCHAR* msg, ...);
	bool		Run() override;
	LRESULT		MsgProc(HWND hWnd,	UINT message,WPARAM wParam,LPARAM lParam)override;
	bool	Init() override;
	bool	Frame()override;
	bool	Render()override;
	bool	Release()override;
	void    ClearD3D11DeviceContext(ID3D11DeviceContext* pd3dDeviceContext);
};

