#pragma once
#include "TStd.h"

enum KeyState
{
	KEY_FREE = 0,
	KEY_UP,
	KEY_PUSH,
	KEY_HOLD,
};
class TInput
{
public:
	float m_fRadianX = 0.0f;
	float m_fRadianY = 0.0f;
	bool  m_bDrag = false;
	DWORD m_dwKeyState[256];
	POINT m_ptMouse;
	POINT m_ptMouseClick;
	POINT m_ptDeltaMouse;
	DWORD m_dwMouseState[3];
	static TInput& Get()
	{
		static TInput theSingle;
		return theSingle;
	}
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	DWORD   GetKey(DWORD dwKey);
	T::TVector2 GetDelta();
private:
	 TInput();
public:
	virtual ~TInput();
};

