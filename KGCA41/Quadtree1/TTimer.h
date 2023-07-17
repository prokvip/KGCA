#pragma once
#include <Windows.h>
#include <iostream>
#pragma comment(lib, "winmm.lib") //timeGetTime()
class TTimer
{
public:
	float	m_fGameTimer;
	float   m_fSecondPerFrame;
	DWORD	m_dwBeforeTime;
	float   m_fFramePerSecond;
	int     m_iFPS;
public:
	int		GetFPS();
public:
	bool	Init();		// 초기화 작업
	bool	Frame();	// 실시간 계산
	bool	Render();	// 실시간 랜더링, 드로우
	bool	Release();	// 객체의 소멸 작업
};

