#pragma once
#include "TCore.h"
#include "TAsyncSelect.h"
#include "TPlayerObj2D.h"
#include "TObjectNpc2D.h"
#include "TMap.h"

class Sample : public TCore
{
public:
	TMap				m_Map;
	TPlayerObj2D		m_PlayerObj;
public:
	std::vector<std::shared_ptr<TObjectNpc2D>> m_NpcLlist;
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

