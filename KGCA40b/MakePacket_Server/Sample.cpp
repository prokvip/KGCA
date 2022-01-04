#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <list>
#include <string>
#include "TProtocol.h"
#pragma comment	(lib, "ws2_32.lib")
struct TUser
{
	SOCKET		m_Sock;
	SOCKADDR_IN m_Addr;
	std::string m_csName;
	short       m_iPort;
	void set(SOCKET sock, SOCKADDR_IN addr)
	{
		m_Sock = sock;
		m_Addr = addr;
		m_csName = inet_ntoa(addr.sin_addr);
		m_iPort = ntohs(addr.sin_port);
	}
};

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

	u_long on = 1;
	ioctlsocket(ListenSock, FIONBIO, &on);

	std::list<TUser> userlist;
	//while (userlist.size() > 2)
	//{
	//	SOCKET clientSock = accept(ListenSock,
	//		(sockaddr*)&clientAddr, &iLen);
	//	if (clientSock == SOCKET_ERROR)
	//	{
	//		int iError = WSAGetLastError();
	//		if (iError != WSAEWOULDBLOCK)
	//		{
	//			std::cout << "ErrorCode=" << iError << std::endl;
	//			break;
	//		}
	//	}
	//	else
	//	{
	//		userlist.push_back(clientSock);
	//		std::cout
	//			<< "ip =" << inet_ntoa(clientAddr.sin_addr)
	//			<< "port =" << ntohs(clientAddr.sin_port)
	//			<< "  " << std::endl;
	//		u_long on = 1;
	//		ioctlsocket(clientSock, FIONBIO, &on);
	//		std::cout << userlist.size() << " 명 접속중.." << std::endl;
	//	}
	//}

	while (1)
	{
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
			TUser user;
			user.set(clientSock, clientAddr);
			userlist.push_back(user);

			std::cout
				<< "ip =" << inet_ntoa(clientAddr.sin_addr)
				<< "port =" << ntohs(clientAddr.sin_port)
				<< "  " << std::endl;
			u_long on = 1;
			ioctlsocket(clientSock, FIONBIO, &on);
			std::cout << userlist.size() << " 명 접속중.." << std::endl;
		}

		if (userlist.size() > 0)
		{
			std::list<TUser>::iterator iter;
			for (iter = userlist.begin(); iter != userlist.end(); )
			{
				TUser user = *iter;
				char szRecvBuffer[256] = { 0, };
				//패킷헤더 받기
				UPACKET recvPacket;
				ZeroMemory(&recvPacket, sizeof(recvPacket));
				int iRecvSize = 0;
				do {
					int iRecvByte = recv(user.m_Sock, szRecvBuffer,
						PACKET_HEADER_SIZE, 0);
					iRecvSize += iRecvByte;
					if (iRecvByte == 0)
					{
						closesocket(user.m_Sock);
						iter = userlist.erase(iter);
						std::cout << user.m_csName << " 접속종료됨." << std::endl;
						continue;
					}
					if (iRecvByte == SOCKET_ERROR)
					{
						int iError = WSAGetLastError();
						if (iError != WSAEWOULDBLOCK)
						{
							iter = userlist.erase(iter);
							std::cout << user.m_csName << " 비정상 접속종료됨." << std::endl;
							break;
						}
						else
						{
							break;
						}
					}
				}while(iRecvSize < PACKET_HEADER_SIZE);
				
				if (iRecvSize == SOCKET_ERROR)
				{
					if (iter != userlist.end())
					{
						iter++;
					}
					continue;
				}
				
				memcpy(&recvPacket.ph, szRecvBuffer, PACKET_HEADER_SIZE);
				// 데이터 받기
				iRecvSize = 0;
				do {
					int iRecvByte = recv(user.m_Sock, recvPacket.msg,
						recvPacket.ph.len- PACKET_HEADER_SIZE- iRecvSize, 0);
					iRecvSize += iRecvByte;
					if (iRecvByte == 0)
					{
						closesocket(user.m_Sock);
						iter = userlist.erase(iter);
						std::cout << user.m_csName << " 접속종료됨." << std::endl;
						continue;
					}
					if (iRecvByte == SOCKET_ERROR)
					{
						int iError = WSAGetLastError();
						if (iError != WSAEWOULDBLOCK)
						{
							iter = userlist.erase(iter);
							std::cout << user.m_csName << " 비정상 접속종료됨." << std::endl;
						}
						else
						{
							iter++;
						}
					}
				} while (iRecvSize < recvPacket.ph.len - PACKET_HEADER_SIZE);

				// 패킷 완성		
				std::list<TUser>::iterator iterSend;
				for (iterSend = userlist.begin();
					iterSend != userlist.end(); )
				{
					TUser user = *iterSend;
					std::cout << szRecvBuffer << "받음" << std::endl;					
					int iSendMsgSize = SendMsg(user.m_Sock, recvPacket);

					std::cout << user.m_Sock << ":" << iSendMsgSize << "보냄." << std::endl;
					if (iSendMsgSize < 0)
					{
						closesocket(user.m_Sock);
						iterSend = userlist.erase(iterSend);
						std::cout << user.m_csName << " 비정상 접속종료됨." << std::endl;						
					}
					else
					{
						iterSend++;
					}
				}
				if (iter != userlist.end())
				{
					iter++;
				}				
			}
		}
	}
	closesocket(ListenSock);
	WSACleanup();
}