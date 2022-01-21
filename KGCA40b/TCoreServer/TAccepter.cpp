#include "TAccepter.h"
#include "TServer.h"
#include <WS2tcpip.h>
bool TAccepter::Run()
{
	TServer* pServer = (TServer*)m_pObject;
	SOCKET sock = m_ListenSock;
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
			std::cout << pServer->m_UserList.size() << " �� ������.." << std::endl;
		}
		Sleep(1);
	}
	return 1;
}
bool TAccepter::Set(int iPort)
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return false;
	}
	m_ListenSock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(iPort);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	int iRet = ::bind(m_ListenSock, (sockaddr*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)
	{
		return false;
	}
	iRet = listen(m_ListenSock, SOMAXCONN);
	if (iRet == SOCKET_ERROR)
	{
		return false;
	}
	std::cout << "���� ������......." << std::endl;
	u_long on = 1;
	ioctlsocket(m_ListenSock, FIONBIO, &on);

	return true;
}
TAccepter::TAccepter()
{

}
TAccepter::TAccepter(LPVOID value) : TThread(value)
{

}

TAccepter::~TAccepter()
{
	closesocket(m_ListenSock);	
	WSACleanup();
}