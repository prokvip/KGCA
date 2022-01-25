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
			std::cout << pServer->m_UserList.size() << " 명 접속중.." << std::endl;
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

	int iType;
	int iSize = sizeof(int);
	int iRet = getsockopt(m_ListenSock, SOL_SOCKET, 
		SO_TYPE,(char*)&iType, &iSize);
	if (iRet == SOCKET_ERROR) {
		return  false; 
	}
	if (iType != SOCK_STREAM) {
		return false;
	}
	// 송수신 버퍼 크기
	/*getsockopt(m_ListenSock, SOL_SOCKET, SO_SNDBUF,	(char*)&iType, &iSize);
	getsockopt(m_ListenSock, SOL_SOCKET, SO_RCVBUF,	(char*)&iType, &iSize);
	iType = 100000;
	setsockopt(m_ListenSock, SOL_SOCKET, SO_SNDBUF,	(char*)&iType, iSize);
	setsockopt(m_ListenSock, SOL_SOCKET, SO_RCVBUF,	(char*)&iType, iSize);
	getsockopt(m_ListenSock, SOL_SOCKET, SO_SNDBUF,	(char*)&iType, &iSize);
	getsockopt(m_ListenSock, SOL_SOCKET, SO_RCVBUF,	(char*)&iType, &iSize);*/
		
	int optval = 1;
	//iRet = setsockopt(m_ListenSock, SOL_SOCKET, SO_REUSEADDR, //SO_EXCLUSIVEADDRUSE,
	//	(char*)&optval, sizeof(optval));
	//if (iRet == SOCKET_ERROR)
	//{
	//	return false;
	//}

	::linger optLinger;
	int iLingerLength = sizeof(optLinger);
	optLinger.l_onoff = 1;
	optLinger.l_linger = 1000;
	iRet = setsockopt(m_ListenSock, SOL_SOCKET, SO_LINGER,
		(char*)&optLinger, iLingerLength);
	if (iRet == SOCKET_ERROR)
	{
		return false;
	}

	optval = 1;
	setsockopt(m_ListenSock, IPPROTO_TCP, TCP_NODELAY, (char*)&optval, sizeof(int));

	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(iPort);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	iRet = ::bind(m_ListenSock, (sockaddr*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)
	{
		return false;
	}
	iRet = listen(m_ListenSock, SOMAXCONN);
	if (iRet == SOCKET_ERROR)
	{
		return false;
	}
	std::cout << "서버 가동중......." << std::endl;
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
	shutdown(m_ListenSock, SD_SEND);
	closesocket(m_ListenSock);	
	WSACleanup();
}