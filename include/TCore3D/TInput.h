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
	DWORD m_dwKeyState[256];
	POINT m_ptMouse;
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
private:
	 TInput();
public:
	virtual ~TInput();
};

