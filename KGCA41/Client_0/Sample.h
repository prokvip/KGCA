#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "TGameCore.h"
#include "TNetwork.h"
class Sample : public TGameCore
{
	TNetwork	m_Net;
	HWND		m_hEdit;
	HWND		m_hListbox;
	HWND		m_hSendBtn;
	bool		m_bNameSend = false;
public:
	void		Print(const WCHAR* msg, ...);
	bool		Run() override;
	void		NetProcess();
	LRESULT		MsgProc(HWND hWnd,	UINT message,WPARAM wParam,LPARAM lParam)override;
	bool	Init() override;
	bool	Frame()override;
	bool	Render()override;
	bool	Release()override;
	void    ClearD3D11DeviceContext(ID3D11DeviceContext* pd3dDeviceContext);
};

