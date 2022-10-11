#pragma once
#include "TStd.h"
#pragma comment(lib, "winmm.lib") // timeGetTime
class TTimer : public TSingleton<TTimer>
{
public:
	LARGE_INTEGER  m_Frequency;
	LARGE_INTEGER  m_Current;
	LARGE_INTEGER  m_Frame;
public:
	float m_fGameTimer = 0.0f;
	float m_fElapseTimer = 10.0f;
	UINT  m_iFPS = 0;
	std::wstring m_szTimer;
private:
	DWORD m_dwBeforeTime; //(1000: 1초)
	UINT  m_iFPSCounter = 0;
	float m_fFPSTimer = 0.0f;
public:
	virtual bool		Init(); // 초기화
	virtual bool		Frame();// 실시간 계산
	virtual bool		Render();// 실시간 랜더링
	virtual bool		Release();// 소멸 및 삭제
};

#define I_Timer TTimer::GetInstance()