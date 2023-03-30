#include "TWorkThread.h"
#include "TServerIOCP.h"
#include "TServer.h"
#include "TDebugString.h"
//#if defined(_WIN64)
bool TWorkThread::Run()
{
	DWORD			bytesTransfer;
	ULONG_PTR		keyValue;
	LPOVERLAPPED	overlapped;
	BOOL rReturn;
	while(m_bLoop)
	{
		rReturn = ::GetQueuedCompletionStatus(
					I_ServerIOCP.m_hIOCP,
					&bytesTransfer,
					&keyValue,
					&overlapped,
					INFINITE );
		
		TUser* pSession = (TUser*)keyValue;
		if (pSession == 0 || pSession->m_Socket == INVALID_SOCKET)
		{
			I_Server.DelUser(pSession->m_Socket);
			continue;
		}
		OVERLAPPED2* pOV = (OVERLAPPED2*)overlapped;

		if( rReturn == TRUE && overlapped != 0 )
		{			
			if (pSession->Dispatch(bytesTransfer, overlapped) == false)
			{
				I_Server.DelUser(pSession->m_Socket);
				
			}
			continue;
		}
		{
			// �ϳ��� �������� RECV�� ���Ŵ������ ���¿���
			// SEND�� �����͸� ������ �Ǹ�
			// RECV�� ������̴� �� ��ҵȴ�.
			if( GetLastError() != ERROR_OPERATION_ABORTED )
			{
				// ������ ���� ���� ����.
				//if(  bytesTransfer == 0)
				{
					// Ŭ���̾�Ʈ ���� ����						
					I_Server.DelUser(pSession->m_Socket);
					//continue;			
				}
			}
		}
		Sleep(1);
	}
	return true;
}
TWorkThread::TWorkThread(void)
{
	m_bLoop = true;
}

TWorkThread::~TWorkThread(void)
{
	m_bLoop = false;
}
