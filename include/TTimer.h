#pragma once
#include "TBasisStd.h"
class CStopwatch
{
public:
	CStopwatch() { QueryPerformanceFrequency(&m_liPerFreq); Start(); }
	void Start() { QueryPerformanceCounter(&m_liPerfStart); }

	__int64 Now()
	{    // Start �� ȣ��� �� ����� �и��ʸ� ����
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
	LARGE_INTEGER m_liPerFreq;    // �ʴ� ���
	LARGE_INTEGER m_liPerfStart;   // ���� ���
};
class TTimer
{
public:
	LARGE_INTEGER	m_Frequency;	// �ʴ� ���ļ� 
	LARGE_INTEGER	m_Current;		// ���� �ð� 
	LARGE_INTEGER	m_Frame;		// ���� �������� �ð� üũ 
	float			m_fSecPerFrame;	// ������ ��� �ð� 
	float			m_fDurationTime;// ����� ��� �ð�
	LARGE_INTEGER	m_FPS;				// FPS üũ Ÿ�̸�		
	int				m_iFramePerSecond;	// �ʴ� ������	
	int				m_iFPSElapse;		// �ʴ� ����� ������ 
	

	LARGE_INTEGER	m_Start;		// �̺�Ʈ Ÿ�̸� ���� �ð�
	LARGE_INTEGER	m_Elapse;		// �̺�Ʈ Ÿ�̸� ��� �ð�
	bool			m_bStarted;			// �̺�Ʈ Ÿ�̸� �۵�����
	float			m_fEventTime;
	

public: 
	// �����ھ�� ����ϴ� �����Լ�
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();

	// ����� �ð��� ���� ���� �Լ���
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
