#pragma once
#include "TServerObj.h"
#include <process.h>
class TThread : public TServerObj
{
public:
	uintptr_t m_hThread;
	unsigned int m_iID;
	bool		m_bStarted;
	LPVOID	m_pObject;
public:
	void Create();
	void Create(LPVOID pValue);
	void Join();
	void Detach();
	virtual bool Run();
	static unsigned int WINAPI Runner(LPVOID param);
public:
	TThread();
	TThread(LPVOID pValue);
	virtual ~TThread();
};

