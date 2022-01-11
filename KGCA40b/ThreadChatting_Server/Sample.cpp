#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "TNetUser.h"
std::list<TNetUser> g_USerList;
CRITICAL_SECTION g_CS;
HANDLE g_hMutex;
int SendMsg(SOCKET sock, char*msg, WORD type)
{
	// 1번 패킷 생성
	UPACKET packet;
	ZeroMemory(&packet, sizeof(packet));
	packet.ph.len = strlen(msg)+PACKET_HEADER_SIZE;
	packet.ph.type = type;
	memcpy(packet.msg, msg, strlen(msg));
	// 2번 패킷 전송 : 운영체제 sendbuffer(short바이트), recvbuffer
	char* pMsg = (char*)&packet;
	int iSendSize = 0;
	do {
		int iSendByte = send(sock, &pMsg[iSendSize],
								packet.ph.len- iSendSize, 0);
		if (iSendByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
		}
		iSendSize += iSendByte;
	} while (iSendSize < packet.ph.len);
	return iSendSize;
}
int SendMsg(SOCKET sock, UPACKET& packet)
{
	char* pMsg = (char*)&packet;
	int iSendSize = 0;
	do {
		int iSendByte = send(sock, &pMsg[iSendSize],
			packet.ph.len - iSendSize, 0);
		if (iSendByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
		}
		iSendSize += iSendByte;
	} while (iSendSize < packet.ph.len);
	return iSendSize;
}
int Broadcast(TNetUser& user)
{
	if (user.m_packetPool.size() > 0)
	{
		std::list<TPacket>::iterator iter;
		for (iter = user.m_packetPool.begin();
			iter != user.m_packetPool.end(); )
		{
			for (TNetUser& senduser : g_USerList)
			{
				int iRet = SendMsg(senduser.m_Sock, (*iter).m_uPacket);
				if (iRet <= 0)
				{
					senduser.m_bConnect = false;
				}
			}
			iter = user.m_packetPool.erase(iter);
		}
	}
	return 1;
}
int RecvUser(TNetUser& user)
{
	char szRecvBuffer[1024] = { 0, };
	int iRecvByte = recv(user.m_Sock, szRecvBuffer, 1024, 0);
	if (iRecvByte == 0)
	{
		return 0;
	}
	if (iRecvByte == SOCKET_ERROR)
	{
		int iError = WSAGetLastError();
		if (iError != WSAEWOULDBLOCK)
		{			
			return -1;
		}
		return 2;
	}
	user.DispatchRead(szRecvBuffer, iRecvByte);
	return 1;
}
DWORD WINAPI RecvThread(LPVOID param)
{
	SOCKET sock = (SOCKET)param;
	while (1)
	{
		//DWORD dwID = GetCurrentThreadId();
		//std::cout << dwID << std::endl;
		//EnterCriticalSection(&g_CS);
		WaitForSingleObject(g_hMutex, INFINITE);
		
		std::list<TNetUser>::iterator userIter;
		for (userIter = g_USerList.begin();
			userIter != g_USerList.end();)
		{
			int iRet = RecvUser(*userIter);
			if (iRet <= 0)
			{
				userIter = g_USerList.erase(userIter);
			}
			else
			{
				userIter++;
			}
		}
		//LeaveCriticalSection(&g_CS);
		ReleaseMutex(g_hMutex);
		Sleep(1);
	}
}
DWORD WINAPI SendThread(LPVOID param)
{
	SOCKET sock = (SOCKET)param;
	while (1)
	{
		WaitForSingleObject(g_hMutex, INFINITE);
		//DWORD dwID = GetCurrentThreadId();
		//std::cout << dwID << std::endl;
		//EnterCriticalSection(&g_CS);		

		std::list<TNetUser>::iterator userIter;
		for (userIter = g_USerList.begin();
			userIter != g_USerList.end();)
		{
			int iRet = Broadcast(*userIter);
			if (iRet <= 0)
			{
				userIter = g_USerList.erase(userIter);
			}
			else
			{
				userIter++;
			}
		}

		//LeaveCriticalSection(&g_CS);
		ReleaseMutex(g_hMutex);

		Sleep(1);
	}
}
void main()
{
	InitializeCriticalSection(&g_CS);
	g_hMutex = CreateMutex(NULL, FALSE, NULL);
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	SOCKET ListenSock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	int iRet = bind(ListenSock, (sockaddr*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}
	iRet = listen(ListenSock, SOMAXCONN);
	if (iRet == SOCKET_ERROR)
	{
		return;
	}

	SOCKADDR_IN clientAddr;
	int iLen = sizeof(clientAddr);

	std::cout	<< "서버 가동중......." << std::endl;

	u_long on = 1;
	ioctlsocket(ListenSock, FIONBIO, &on);

	DWORD ThreadId;
	HANDLE hThreadRecv = ::CreateThread(
		0,
		0,
		RecvThread,// 반환
		(LPVOID)ListenSock,
		0,
		&ThreadId
	);
	CloseHandle(hThreadRecv);
	DWORD ThreadIdSend;
	HANDLE hThreadSend = ::CreateThread(
		0,
		0,
		SendThread,// 반환
		(LPVOID)ListenSock,
		0,
		&ThreadIdSend
	);
	CloseHandle(hThreadSend);

	while (1)
	{
		//DWORD dwID = GetCurrentThreadId();
		//std::cout << dwID << " MainThread" << std::endl;

		SOCKET clientSock = accept(ListenSock,
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
			TNetUser user;
			user.set(clientSock, clientAddr);
			//EnterCriticalSection(&g_CS);
			WaitForSingleObject(g_hMutex, INFINITE);
				g_USerList.push_back(user);
			//LeaveCriticalSection(&g_CS);
			ReleaseMutex(g_hMutex);

			std::cout
				<< "ip =" << inet_ntoa(clientAddr.sin_addr)
				<< "port =" << ntohs(clientAddr.sin_port)
				<< "  " << std::endl;
			u_long on = 1;
			ioctlsocket(clientSock, FIONBIO, &on);
			std::cout << g_USerList.size() << " 명 접속중.." << std::endl;
		}
		Sleep(1);
	}
	closesocket(ListenSock);
	WSACleanup();

	DeleteCriticalSection(&g_CS);
	CloseHandle(g_hMutex);
}