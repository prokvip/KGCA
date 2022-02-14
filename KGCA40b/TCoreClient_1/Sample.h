#pragma once
#include "TCore.h"
#include "TAsyncSelect.h"
#include "TPlayerObj2D.h"
class Sample : public TCore
{
public:
	TPlayerObj2D		m_PlayerObj;
public:
	std::vector<TDxObject> m_ObjectList;
	TAsyncSelect		m_Net;
	HWND				m_hEdit;
	HWND				m_hButton;
	HWND				m_hListBox;
	int					m_iChatCnt;
public:
	virtual bool	Init()  override;
	virtual bool	Frame()  override;
	virtual bool	Render()  override;
	virtual bool	Release()  override;
	virtual LRESULT  MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	Sample();
	~Sample();
};

