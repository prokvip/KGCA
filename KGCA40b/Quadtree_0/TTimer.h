#pragma once
#include <windows.h>
#pragma comment	(lib, "Winmm.lib")
class TTimer
{
public:
	float   m_fSecondPerFrame;
	float   m_fTimer;
	DWORD	m_dwBeforeTime;
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
};

