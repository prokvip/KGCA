#include "TAccepter.h"
#include "TServer.h"
#include <WS2tcpip.h>
bool TAccepter::Run()
{
	TServer* pServer = (TServer*)m_pObject;
	SOCKET sock = pServer->m_ListenSock;
	SOCKADDR_IN clientAddr;
	int iLen = sizeof(clientAddr);
	while (1)
	{
		//DWORD dwID = GetCurrentThreadId();
		//std::cout << dwID << " MainThread" << std::endl;
		SOCKET clientSock = accept(sock,
			(sockaddr*)&clientAddr, &iLen);
		if (clientSock == SOCKET_ERROR)
		{
			int iError = WSAGetLastError();
			if (iError != WSAEWOULDBLOCK)
			{
				std::cout << "ErrorCode=" << iError << std::endl;
				break;
			}
		}
		else
		{
			pServer->AddUser(clientSock, clientAddr);		
			std::cout << pServer->m_UserList.size() << " 명 접속중.." << std::endl;
		}
		Sleep(1);
	}
	//SetEvent();
	return 1;
}
TAccepter::TAccepter()
{

}
TAccepter::TAccepter(LPVOID value) : TThread(value)
{

}