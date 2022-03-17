#pragma once
#include <windows.h>
#pragma comment	(lib, "Winmm.lib")
class TTimer
{
public:
	int		m_iFPS;
	float   m_fSecondPerFrame;
	float   m_fTimer;
	DWORD	m_dwBeforeTime;

	float   m_fFPSTimer = 0.0f;	
	int		m_iFPSCounter = 0;
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
};

