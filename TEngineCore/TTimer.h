#pragma once
#include "TStd.h"

// 1,실행시간 누적(게임플레이 시간)
// 2,1프레임의 경과 시간
// 3,1초에 실행된 카운터(FPS)
class TTimer
{
private:
	LARGE_INTEGER m_Frequency;// 주파수
	LARGE_INTEGER m_Current;
	LARGE_INTEGER m_Frame;	
	int				m_iTmpCounter;
public:
	int	  m_iFPS;
	float m_fSecPerFrame;
	float m_fGameTimer;
	TCHAR m_szTimerString[MAX_PATH] = { 0, };
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
public:
	TTimer();
	virtual ~TTimer();
};

