#pragma once
#include "TThread.h"

class TWorkThread : public TThread
{
public:
	bool		m_bLoop;
public:
	bool		Init();
	bool		Run();
	bool		Release();
public:
	TWorkThread(void);
	virtual ~TWorkThread(void);
};
