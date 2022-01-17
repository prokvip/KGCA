#include "TLobbyServer.h"
DWORD WINAPI WorkerThread(LPVOID param)
{
	TLobbyServer* pServer = (TLobbyServer*)param;
	DWORD dwTransfer;
	ULONG_PTR KeyValue;
	OVERLAPPED* pOverlapped;
	while (1)
	{
		if (WaitForSingleObject(pServer->m_hKillEvent, 1) == WAIT_OBJECT_0)
		{
			break;
		}
		// 완료 큐에 데이터가 있으면 작업시작
		BOOL bReturn = ::GetQueuedCompletionStatus(
			pServer->g_hIOCP,
			&dwTransfer,
			&KeyValue,
			&pOverlapped, 1);
		
		TChatUser* pUser = (TChatUser*)KeyValue;
		TOV* pOV = (TOV*)pOverlapped;
		if (bReturn == TRUE && pUser && pOV )
		{
			if (dwTransfer == 0 && pOV->type == 1000)
			{
				pUser->m_bConnect = false;
			}
			else
			{
				pUser->Dispatch(dwTransfer, pOV);
			}
		}
		else
		{
			if (GetLastError() != WAIT_TIMEOUT)
			{
				::SetEvent(pServer->m_hKillEvent);
				break;
			}
		}
	}
	return 1;
}
bool TLobbyServer::AddUser(SOCKET sock, SOCKADDR_IN clientAddr)
{
	TChatUser* user= new TChatUser;
	user->set(sock, clientAddr);
	u_long on = 1;
	ioctlsocket(sock, FIONBIO, &on);
	
	EnterCriticalSection(&m_cs);
		m_UserList.push_back(user);
	LeaveCriticalSection(&m_cs);

	::CreateIoCompletionPort((HANDLE)sock, g_hIOCP, (ULONG_PTR)user, 0);
	user->Recv();

	char ip[INET_ADDRSTRLEN];
	std::cout
		<< "ip =" <<
		inet_ntop(AF_INET, &(clientAddr.sin_addr),
			ip, INET_ADDRSTRLEN)
		<< "port =" << ntohs(clientAddr.sin_port)
		<< "  " << std::endl;
	return true;
}
bool TLobbyServer::InitServer(int iPort)
{
	TServer::InitServer(iPort);
	g_hIOCP = ::CreateIoCompletionPort(	INVALID_HANDLE_VALUE, 0, 0, 0);	

	for (int iThread = 0;	iThread < MAX_WORKER_THREAD;  iThread++)
	{
		DWORD id;
		g_hWorkThread[iThread] = ::CreateThread(0, 0, 
			WorkerThread,
			this, 0, &id);
	}
	return true;
}
bool TLobbyServer::Run()
{
	while (1)
	{
		EnterCriticalSection(&m_cs);			
		for (TNetUser* tUser : m_UserList)
		{
			TChatUser* pChat = (TChatUser*)tUser;
			if (pChat->m_packetPool.size() > 0)
			{
				Broadcast(tUser);
			}			
		}
		std::list<TNetUser*>::iterator iter;
		for (iter = m_UserList.begin();
			iter != m_UserList.end();)
		{
			if ((*iter)->m_bConnect == false)
			{
				(*iter)->DisConnect();
				delete (*iter);
				iter = m_UserList.erase(iter);
			}
			else
			{
				iter++;
			}
		}
		
		LeaveCriticalSection(&m_cs);
		Sleep(1);
	}
	return true;
}
bool TLobbyServer::Release()
{
	CloseHandle(g_hIOCP);
	TServer::Release();
	return true;
}