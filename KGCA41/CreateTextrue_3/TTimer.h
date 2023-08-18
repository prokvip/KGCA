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
	bool	Init();		// �ʱ�ȭ �۾�
	bool	Frame();	// �ǽð� ���
	bool	Render();	// �ǽð� ������, ��ο�
	bool	Release();	// ��ü�� �Ҹ� �۾�
};

