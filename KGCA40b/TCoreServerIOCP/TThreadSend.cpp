#include "TThreadSend.h"
#include "TServer.h"
bool TThreadSend::Run()
{
	TServer* pServer = (TServer*)m_pObject;
	SOCKET sock = pServer->m_ListenSock;
	while (1)
	{
		WaitForSingleObject(pServer->g_hMutex, INFINITE);
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
		ReleaseMutex(pServer->g_hMutex);
		Sleep(1);
	}
	return 1;
}
TThreadSend::TThreadSend()
{

}
TThreadSend::TThreadSend(LPVOID value) : TThread(value)
{

}