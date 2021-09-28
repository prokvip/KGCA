#pragma once
#include "TStd.h"
// Ű����, ���콺 ->������API ���
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
	POINT   m_ptPos;
public:
	DWORD   GetKey(DWORD dwKey);
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
private:
	TInput();
public:
	~TInput();
};

#define g_Input TInput::Get()