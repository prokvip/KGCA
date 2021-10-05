#pragma once
#include "TStd.h"
// 키보드, 마우스 ->윈도우API 사용
enum  KeyState
{
	KEY_FREE = 0,
	KEY_UP,
	KEY_PUSH,
	KEY_HOLD,
};
class TInput : public TSingleton<TInput>
{
	friend class TSingleton<TInput>;
//public:
//	static TInput* Get()
//	{
//		static TInput gInput;
//		return &gInput;
//	}
private:
	DWORD	m_dwKeyState[256];
public:
	POINT   m_ptBeforePos;
	POINT   m_ptPos;
	POINT   m_pDragDown;
	POINT   m_pDrag;
	bool    m_bDrag;
	bool    m_bMove;
	int		m_iWheel = 0;
public:
	DWORD   GetKey(DWORD dwKey);
	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
public:
	void   OnMove(int iX, int iY);
	void   OnBegin(int iX, int iY);
	void   OnEnd();
private:
	TInput();
public:
	~TInput();
};

#define g_Input TInput::Get()