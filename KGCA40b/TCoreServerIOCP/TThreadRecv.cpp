#include "TThreadRecv.h"
#include "TServer.h"

bool TThreadRecv::Run()
{
	TServer* pServer = (TServer*)m_pObject;
	SOCKET sock = pServer->m_ListenSock;
	while (1)
	{		
		/*if (WaitForSingleObject(pServer->g_hKillEvent, 1))
		{
			break;
		}*/
		
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
		ReleaseMutex(pServer->g_hMutex);
		Sleep(1);
	}
	return 1;
}

TThreadRecv::TThreadRecv()
{

}
TThreadRecv::TThreadRecv(LPVOID value) : TThread(value)
{

}