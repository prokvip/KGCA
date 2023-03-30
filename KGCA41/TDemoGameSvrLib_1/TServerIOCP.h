#pragma once
#include "TServerObj.h"
#include "TWorkThread.h"

#define WORKER_THREAD_COUNT 2

class TServerIOCP : public TSingleton<TServerIOCP>,
					public TServerObj
{
private:
	friend class TSingleton<TServerIOCP>;
public:
	HANDLE		 m_hIOCP;
	TWorkThread	 m_WorkerThread[WORKER_THREAD_COUNT];
public :
	bool		Init();
	bool		Run();
	bool		Release();
	//ULONG_PTR 32bit, 64bit ¡÷¿«
	void		AddhandleToIOCP( 
					HANDLE hClientSocket, 
		ULONG_PTR dwReturnKey );
public:
	TServerIOCP(void);
	virtual ~TServerIOCP(void);
};
#define I_ServerIOCP TServerIOCP::GetInstance()