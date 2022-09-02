#pragma once
#include "TStd.h"
enum KeyState
{
	KEY_FREE = 0,
	KEY_UP, 
	KEY_PUSH,
	KEY_HOLD,
};
class TInput : public TSingleton<TInput>
{
private:
	DWORD		m_dwKeyState[256]; // mouse 버튼 + 키버튼
public:
	POINT		m_ptPos; // mouse position
public:
	virtual bool		Init(); // 초기화
	virtual bool		Frame();// 실시간 계산
	virtual bool		Render();// 실시간 랜더링
	virtual bool		Release();// 소멸 및 삭제
public:
	DWORD  GetKey(DWORD dwKey);
};
#define I_Input TInput::GetInstance()
