#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <list>
#include <string>
#include "TPacket.h"
#pragma comment	(lib, "ws2_32.lib")
struct TUser
{
	SOCKET		m_Sock;
	SOCKADDR_IN m_Addr;
	std::string m_csName;
	short       m_iPort;
	// buffer
	char m_szRecvBuffer[2048];
	int  m_iPacketPos; // 패킷의 시작주소
	int  m_iWritePos;  // 현재의 저장 위치
	int  m_iReadPos;
	std::list<TPacket> m_packetPool;
	int DispatchRead(char* szRecvBuffer, int iRecvByte)
	{
		//p1(m_iPacketPos)  p2(2.1)       pn   (m_ReadPos)
		//2035 ~ 2038 ~ 22  ~ 50  ~  2028 ~ 2038 ~ 2048 
		//0 ~ 5 ~ iRecvByte
		if (m_iWritePos+ iRecvByte >= 2048)
		{
			if (m_iReadPos > 0)
			{
				memmove(&m_szRecvBuffer[0], &m_szRecvBuffer[m_iPacketPos], m_iReadPos);
			}
			m_iPacketPos = 0;
			m_iWritePos = m_iReadPos;
		}
		memcpy(&m_szRecvBuffer[m_iWritePos], szRecvBuffer, iRecvByte);
		m_iWritePos += iRecvByte;// 버퍼에 이전에 저장된 위치
		m_iReadPos += iRecvByte; // 패킷시작 위치로부터 받은 바이트

		if (m_iReadPos >= PACKET_HEADER_SIZE)
		{
			// 패킷 해석 가능
			UPACKET* pPacket = (UPACKET*)&m_szRecvBuffer[m_iPacketPos];
			// 적어도 1개의 패킷은 도착했다.
			if (pPacket->ph.len <= m_iReadPos)
			{
				do {
					TPacket tPacket(pPacket->ph.type);
					memcpy( &tPacket.m_uPacket, 
							&m_szRecvBuffer[m_iPacketPos],
							pPacket->ph.len);
					m_packetPool.push_back(tPacket);

					// 다음패킷 처리
					m_iPacketPos += pPacket->ph.len;
					m_iReadPos -= pPacket->ph.len;

					pPacket = (UPACKET*)&m_szRecvBuffer[m_iPacketPos];
				} while (pPacket->ph.len <= m_iReadPos);
			}			
		}		
		return 1;
	}

	void set(SOCKET sock, SOCKADDR_IN addr)
	{
		ZeroMemory(m_szRecvBuffer, sizeof(char) * 2048);
		m_iPacketPos =  0;
		m_iWritePos = 0;
		m_iReadPos = 0;
		int  m_iWritePos;  // 현재의 저장 위치
		int  m_iReadPos;

		m_Sock = sock;
		m_Addr = addr;
		m_csName = inet_ntoa(addr.sin_addr);
		m_iPort = ntohs(addr.sin_port);
	}
};
std::list<TUser> userlist;

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

int AddUser(SOCKET sock)
{
	SOCKADDR_IN clientAddr;
	int iLen = sizeof(clientAddr);
	SOCKET clientSock = accept(sock,
		(sockaddr*)&clientAddr, &iLen);
	if (clientSock == SOCKET_ERROR)
	{		
		return -1;
	}
	else
	{
		TUser user;
		user.set(clientSock, clientAddr);
		userlist.push_back(user);

		std::cout
			<< "ip =" << inet_ntoa(clientAddr.sin_addr)
			<< "port =" << ntohs(clientAddr.sin_port)
			<< "  " << std::endl;
		std::cout << userlist.size() << " 명 접속중.." << std::endl;
	}
}
int RecvUser(TUser& user)
{
	char szRecvBuffer[1024] = { 0, };	
	int iRecvByte = recv(user.m_Sock, szRecvBuffer, 1024, 0);
	if (iRecvByte == 0)
	{
		closesocket(user.m_Sock);
		return 0;
	}
	if (iRecvByte == SOCKET_ERROR)
	{
		return -1;
	}	
	user.DispatchRead(szRecvBuffer, iRecvByte);
	return 1;
}
void main()
{
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
	if (iRet == SOCKET_ERROR)  return;
	iRet = listen(ListenSock, SOMAXCONN);
	if (iRet == SOCKET_ERROR)  return;

	SOCKADDR_IN clientAddr;
	int iLen = sizeof(clientAddr);

	std::cout
		<< "서버 가동중......." << std::endl;

	//u_long on = 1;
	//ioctlsocket(ListenSock, FIONBIO, &on);

	FD_SET rSet;
	FD_SET wSet;
	timeval timeout;
	while (1)
	{
		FD_ZERO(&rSet);
		FD_ZERO(&wSet);
		FD_SET(ListenSock, &rSet);
		for (TUser& user : userlist)
		{
			FD_SET(user.m_Sock, &rSet);
			// 만약 user에서 보낸 페킷이 있으면
			if (user.m_packetPool.size() > 0)
			{
				FD_SET(user.m_Sock, &wSet);
			}
		}
		int iRet = select(0, &rSet,
			NULL,
			 NULL,
			NULL);
		if (iRet == 0)
		{
			continue;
		}
		if (FD_ISSET(ListenSock, &rSet))
		{
			if (AddUser(ListenSock)< 0)
			{
				break;
			}
		}
		for (TUser& user : userlist)
		{
			if (FD_ISSET(user.m_Sock, &rSet))
			{
				int iRet= RecvUser(user);
			}
		}
		for (TUser& user : userlist)
		{
			if (FD_ISSET(user.m_Sock, &wSet))
			{
				if (user.m_packetPool.size() > 0)
				{
					for (TUser& senduser : userlist)
					{
						std::list<TPacket>::iterator iter;
						for (iter = senduser.m_packetPool.begin();
								iter != senduser.m_packetPool.end();
							iter++)
						{
							TPacket* tPacket = (TPacket*)&iter;
							SendMsg(senduser.m_Sock, tPacket->m_uPacket);
						}
					}
				}
			}
		}
	}
	closesocket(ListenSock);
	WSACleanup();
}