#pragma once
#include "TStd.h"

// 1,����ð� ����(�����÷��� �ð�)
// 2,1�������� ��� �ð�
// 3,1�ʿ� ����� ī����(FPS)
class TTimer
{
private:
	LARGE_INTEGER m_Frequency;// ���ļ�
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

