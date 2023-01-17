#pragma once
#include "TSessionMgr.h"
#define MAX_WORKER_THREAD 3
class TIocp
{
	HANDLE  m_hIOCP;
	HANDLE  m_hEventFinish;
	unsigned long m_hWorkThread[MAX_WORKER_THREAD];
	unsigned int  m_iThreadID[MAX_WORKER_THREAD];
public:
	bool	SetBind(SOCKET sock, ULONG_PTR key);
	static unsigned WINAPI WorkProc(LPVOID arg);
public:
	bool	Init();
	bool	Run();
	bool	Release();
};

