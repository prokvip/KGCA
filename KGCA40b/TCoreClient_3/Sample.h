#pragma once
#include "TCore.h"
#include "TAsyncSelect.h"
#include "TIntroWorld.h"
#include "TLoginWorld.h"
#include "TZoneWorld.h"

class Sample : public TCore
{
public:

	TIntroWorld			m_IntroWorld;
	//TLoginWorld			m_LoginWorld;
	TZoneWorld			m_ZoneWorld;
public:
	TAsyncSelect		m_Net;
	HWND				m_hEdit;
	HWND				m_hButton;
	HWND				m_hListBox;
	int					m_iChatCnt;
public:
	virtual void	CreateResizeDevice(UINT iWidth, UINT iHeight) override;
	virtual void	DeleteResizeDevice(UINT iWidth, UINT iHeight) override;
	virtual bool	Init()  override;
	virtual bool	Frame()  override;
	virtual bool	Render()  override;
	virtual bool	Release()  override;
	virtual LRESULT  MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	Sample();
	~Sample();
};

