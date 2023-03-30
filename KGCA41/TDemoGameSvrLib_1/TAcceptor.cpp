#include "TAcceptor.h"
#include "TDebugString.h"
#include "TServer.h"
#include "TLock.h"
#include "TServerIOCP.h"
#include <errno.h>
int TAcceptor::GetIPList(vector<string>& ipList)
{
	char szHostName[1024];
	string strHostName;
	string strIPAddr = "";
	int  i;
	WSADATA WsaData;
	if(WSAStartup(0x202, &WsaData) == SOCKET_ERROR)
	{ 
		return 0;
	}
	if (! ::gethostname(szHostName, 1024))
	{
		strHostName = szHostName;
		struct hostent* pHost;
		pHost = gethostbyname(szHostName);
		
		if (pHost != NULL)
		{
			
			for(i=0;i<255;i++)
			{
				if (pHost->h_addr_list[i])
				{
					BYTE    bAddr[4];
					char    strIPAddr[256] = {0,};
					bAddr[0] = (BYTE) pHost->h_addr_list[i][0];
					bAddr[1] = (BYTE) pHost->h_addr_list[i][1];
					bAddr[2] = (BYTE) pHost->h_addr_list[i][2];
					bAddr[3] = (BYTE) pHost->h_addr_list[i][3];
					sprintf_s(strIPAddr, "%d.%d.%d.%d", bAddr[0], bAddr[1], bAddr[2], bAddr[3]);
					ipList.push_back(strIPAddr);
				}
				else
				{
					break;
				}
			}
		}
	}
	WSACleanup();
	return (int)ipList.size();
}
bool TAcceptor::Set( int iPort, const char* strAddress )
{
	int     iRet;
	//m_ListenSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	m_ListenSock = socket(AF_INET, SOCK_STREAM, 0 );
	if( m_ListenSock == INVALID_SOCKET )
	{
		return false;
	}

	int optval = 1;
	setsockopt( m_ListenSock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));

	int option = TRUE;
	setsockopt(m_ListenSock, IPPROTO_TCP, TCP_NODELAY, (const char*)&option, sizeof(option));

	//LINGER  ling = { 0, };
	//ling.l_onoff = 1;   // LINGER 옵션 사용 여부  
	//ling.l_linger = 0;  // LINGER Timeout 설정  
	//setsockopt(m_ListenSock, SOL_SOCKET, SO_LINGER, (CHAR*)&ling, sizeof(ling));


	SOCKADDR_IN serveraddr;
	ZeroMemory( &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family	= AF_INET;
	if( strAddress == NULL)
	{
		serveraddr.sin_addr.s_addr= htonl(INADDR_ANY);
	}
	else
	{
		serveraddr.sin_addr.s_addr= inet_addr(strAddress);
	}
	serveraddr.sin_port		= htons(iPort);
	iRet = bind(m_ListenSock, (SOCKADDR*)&serveraddr, sizeof(serveraddr) );
	if( iRet == SOCKET_ERROR )
	{
		return false;
	}

	iRet = listen( m_ListenSock, SOMAXCONN );
	if( iRet == SOCKET_ERROR )
	{
		return false;
	}

	CreateThread();
	return true;
}
bool TAcceptor::Run()
{
	SOCKADDR_IN clientaddr;
	int			addrlen;
	SOCKET		client_sock;
	while(m_bStarted)
	{
		addrlen = sizeof( clientaddr );
		client_sock = accept( m_ListenSock, (SOCKADDR*)&clientaddr, &addrlen );
		if( client_sock == INVALID_SOCKET )
		{
			continue;
		}
		{
			DWORD dwError = 0L;
			tcp_keepalive sKA_Settings = { 0 }, sReturned = { 0 };
			sKA_Settings.onoff = 1;
			sKA_Settings.keepalivetime = 50000;        // 5초 간격으로 검사한다.(패킷교환이 없는 시간이 5초 보다 많을 시)
			sKA_Settings.keepaliveinterval = 3000;    // 응답이 없을시 3초 간격으로 10회 재전송 후 종료처리한다.			

			// 연결을 받은 서버 소켓의 특성을 클라이언트 소켓이 상속하도록 한다.  
			setsockopt(m_ListenSock
				,SOL_SOCKET
				,SO_UPDATE_ACCEPT_CONTEXT
				,(const char*)&client_sock
				,sizeof(SOCKET));
		
			DWORD dwBytes;
			if (WSAIoctl(client_sock, SIO_KEEPALIVE_VALS, &sKA_Settings,
				sizeof(sKA_Settings), &sReturned, sizeof(sReturned), &dwBytes, NULL, NULL) != 0)
			{				
				T_ERR(1);
			}
			TCHAR buf[INET_ADDRSTRLEN] = { 0, };
			InetNtop(AF_INET, &clientaddr.sin_addr, buf, sizeof(buf));
			T_LOG("[IP:%s,PORT:%d] ACCEPTING \r\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
			ISessionMgr.CreateNewSession(client_sock, clientaddr);
		}
		Sleep(1);
	}
	m_bStarted = false;
	// 스레드 시작 함수가 리턴되면 스레드는 소멸됨.
	return 0;
}
TAcceptor::TAcceptor(void)
{
}

TAcceptor::~TAcceptor(void)
{
}
