#include "TServerIOCP.h"

bool TServerIOCP::Init()
{
	m_hIOCP = ::CreateIoCompletionPort( 
					INVALID_HANDLE_VALUE,
					0,
					0, 
					WORKER_THREAD_COUNT );
	for( int iThread=0; iThread < WORKER_THREAD_COUNT; iThread++ )
	{
		m_WorkerThread[iThread].CreateThread();
	}
	return true;
}
bool	TServerIOCP::Run()
{
	return true;
}
bool	TServerIOCP::Release()
{
	return true;
}
void   TServerIOCP::AddhandleToIOCP(HANDLE hClientSocket, ULONG_PTR dwReturnKey )
{
	::CreateIoCompletionPort(hClientSocket, m_hIOCP, dwReturnKey, 0 );
}
TServerIOCP::TServerIOCP(void)
{
	m_hIOCP = NULL;
}

TServerIOCP::~TServerIOCP(void)
{
}
