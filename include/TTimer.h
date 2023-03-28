#pragma once
#include "TBasisStd.h"
class CStopwatch
{
public:
	CStopwatch() { QueryPerformanceFrequency(&m_liPerFreq); Start(); }
	void Start() { QueryPerformanceCounter(&m_liPerfStart); }

	__int64 Now()
	{    // Start 가 호출된 후 경과한 밀리초를 리턴
		LARGE_INTEGER liPerfNow;
		QueryPerformanceCounter(&liPerfNow);
		__int64 qwElapsedTime = (((liPerfNow.QuadPart - m_liPerfStart.QuadPart)));// / m_liPerFreq.QuadPart);
		return qwElapsedTime;
	}
	void Output(const TCHAR* pData)
	{
#ifdef _DEBUG
		TCHAR  strMessage[MAX_PATH] = { 0, };
		__int64 qwElapsedTime = Now();
		//float Second = static_cast<float>(qwElapsedTime) / static_cast<float>(m_liPerFreq.QuadPart);
		_stprintf_s(strMessage, _T("<=====%s[%lld]====>"), pData, qwElapsedTime);
		OutputDebugString(strMessage);
#endif
	}
private:
	LARGE_INTEGER m_liPerFreq;    // 초당 계수
	LARGE_INTEGER m_liPerfStart;   // 시작 계수
};
class TTimer
{
public:
	LARGE_INTEGER	m_Frequency;	// 초당 주파수 
	LARGE_INTEGER	m_Current;		// 현재 시간 
	LARGE_INTEGER	m_Frame;		// 이전 프레임의 시간 체크 
	float			m_fSecPerFrame;	// 프레임 경과 시간 
	float			m_fDurationTime;// 실행된 경과 시간
	LARGE_INTEGER	m_FPS;				// FPS 체크 타이머		
	int				m_iFramePerSecond;	// 초당 프레임	
	int				m_iFPSElapse;		// 초당 경과된 프레임 
	

	LARGE_INTEGER	m_Start;		// 이벤트 타이머 시작 시간
	LARGE_INTEGER	m_Elapse;		// 이베트 타이머 경과 시간
	bool			m_bStarted;			// 이벤트 타이머 작동여부
	float			m_fEventTime;
	

public: 
	// 엔진코어에서 사용하는 지원함수
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();

	// 경과한 시간에 대한 지원 함수들
	void	Reset();
	void	Start();
	void	Stop();
	bool	IsStarted() const;
	float	GetElapsedTime();

	// FPS
	int				GetFPS();
	float			GetSPF();

public: 
	TTimer();  
	~TTimer();
};
