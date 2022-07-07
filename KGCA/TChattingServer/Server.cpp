#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "TServer.h"

DWORD WINAPI RecvThread(LPVOID param)
{
	TServer* pServer = (TServer*)param;
	SOCKET sock = pServer->m_ListenSock;
	while (1)
	{
		//DWORD dwID = GetCurrentThreadId();
		//std::cout << dwID << std::endl;
		//EnterCriticalSection(&g_CS);
		WaitForSingleObject(pServer->g_hMutex, INFINITE);

		std::list<TNetUser>::iterator userIter;
		for (userIter = pServer->g_UserList.begin();
			userIter != pServer->g_UserList.end();)
		{
			int iRet = pServer->RecvUser(*userIter);
			if (iRet <= 0)
			{
				userIter = pServer->g_UserList.erase(userIter);
			}
			else
			{
				userIter++;
			}
		}
		//LeaveCriticalSection(&pServer->g_CS);
		ReleaseMutex(pServer->g_hMutex);
		Sleep(1);
	}
}
DWORD WINAPI SendThread(LPVOID param)
{
	TServer* pServer = (TServer*)param;
	SOCKET sock = pServer->m_ListenSock;
	while (1)
	{
		WaitForSingleObject(pServer->g_hMutex, INFINITE);
		//DWORD dwID = GetCurrentThreadId();
		//std::cout << dwID << std::endl;
		//EnterCriticalSection(&g_CS);		

		std::list<TNetUser>::iterator userIter;
		for (userIter = pServer->g_UserList.begin();
			userIter != pServer->g_UserList.end();)
		{
			int iRet = pServer->Broadcast(*userIter);
			if (iRet <= 0)
			{
				userIter = pServer->g_UserList.erase(userIter);
			}
			else
			{
				userIter++;
			}
		}

		//LeaveCriticalSection(&g_CS);
		ReleaseMutex(pServer->g_hMutex);

		Sleep(1);
	}
}
void main()
{
	TServer server;
	server.InitServer(9000);
	DWORD ThreadId;
	HANDLE hThreadRecv = ::CreateThread(0, 0, RecvThread, (LPVOID)&server, 0, &ThreadId);
	DWORD ThreadIdSend;
	HANDLE hThreadSend = ::CreateThread(0, 0, SendThread, (LPVOID)&server, 0, &ThreadIdSend);
	server.Run();
	server.Release();
	CloseHandle(hThreadRecv);
	CloseHandle(hThreadSend);
}