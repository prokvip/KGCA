#include "TIocp.h"
unsigned WINAPI TIocp::WorkProc(LPVOID arg)
{
	TIocp* pIocp = (TIocp*)arg;
	DWORD dwTransfer;
	ULONG_PTR KeyValue;
	OVERLAPPED* pOV;
	while (1)
	{
		DWORD dwEvent = WaitForSingleObject(pIocp->m_hEventFinish, 0);
		if (dwEvent == WAIT_OBJECT_0)
		{
			break;
		}
		// 비동기 읽기 완성 여부 판단
		BOOL bRet = ::GetQueuedCompletionStatus(pIocp->m_hIOCP,
			&dwTransfer,
			&KeyValue,
			&pOV, INFINITE);

		OVERLAPPED2* pOV2 = (OVERLAPPED2*)pOV;
		TSessionUser* user = (TSessionUser*)KeyValue;
		if (bRet == TRUE)
		{
			// 읽기 또는 쓰기 비동기 완성
			if (OVERLAPPED2::MODE_RECV == pOV2->iType)
			{
				user->DispatchRead(dwTransfer, pOV2);
				user->RecvMsg();
			}
			if (OVERLAPPED2::MODE_SEND == pOV2->iType)
			{				
				user->DispatchSend(dwTransfer, pOV2);
			}
		}
		else
		{
			DWORD dwError = GetLastError();
			if (dwError == ERROR_NETNAME_DELETED)
			{
				user->m_bDisConnect = true;
			}
		}
	}
	return 0;
}
bool	TIocp::Init()
{
	m_hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	m_hEventFinish = ::CreateEvent(0, TRUE, FALSE, 0);
	for (int iThread = 0; iThread < MAX_WORKER_THREAD; iThread++)
	{
		m_hWorkThread[iThread] =
			_beginthreadex(NULL, 0, WorkProc, this, 0, 
				&m_iThreadID[iThread]);

	}
	return true;
}
bool	TIocp::SetBind(SOCKET sock, ULONG_PTR key)
{
	::CreateIoCompletionPort((HANDLE)sock, m_hIOCP, key, 0);
	return true;
}
bool	TIocp::Run()
{
	return true;
}
bool	TIocp::Release()
{
	CloseHandle(m_hIOCP);
	for (int iThread = 0; iThread < MAX_WORKER_THREAD; iThread++)
	{
		CloseHandle((HANDLE)m_hWorkThread[iThread]);
	}
	return true;
}