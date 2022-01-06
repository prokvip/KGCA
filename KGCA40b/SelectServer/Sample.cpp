#include "TNetwork.h"

std::list<TNetUser> userlist;

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
		TNetUser user;
		user.set(clientSock, clientAddr);		
		userlist.push_back(user);
		std::cout
			<< "ip =" << inet_ntoa(clientAddr.sin_addr)
			<< "port =" << ntohs(clientAddr.sin_port)
			<< "  " << std::endl;
		std::cout << userlist.size() << " 명 접속중.." << std::endl;
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
		return -1;
	}	
	user.DispatchRead(szRecvBuffer, iRecvByte);
	return 1;
}

void main()
{
	TNetwork net;
	net.InitNetwork();
	net.InitServer(SOCK_STREAM, 10000);	

	SOCKADDR_IN clientAddr;
	int iLen = sizeof(clientAddr);
	std::cout<< "서버 가동중......." << std::endl;
	//u_long on = 1;
	//ioctlsocket(m_ListenSock, FIONBIO, &on);

	FD_SET rSet;
	FD_SET wSet;
	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 1;
	while (1)
	{
		FD_ZERO(&rSet);
		FD_ZERO(&wSet);
		FD_SET(net.m_ListenSock, &rSet);
		std::list<TNetUser>::iterator userIter;
		for (userIter = userlist.begin();
			 userIter != userlist.end();)
		{
			if ((*userIter).m_bConnect == false)
			{
				std::cout << (*userIter).m_csName << " 접속종료됨." << std::endl;
				userIter = userlist.erase(userIter);	
				continue;
			}
			FD_SET((*userIter).m_Sock, &rSet);
			// 만약 user에서 보낸 페킷이 있으면
			if ((*userIter).m_packetPool.size() > 0)
			{
				FD_SET((*userIter).m_Sock, &wSet);
			}
			userIter++;
		}
		int iRet = select(0, &rSet, &wSet, NULL, &timeout);
		if (iRet == 0)
		{
			continue;
		}
		if (FD_ISSET(net.m_ListenSock, &rSet))
		{
			if (AddUser(net.m_ListenSock)<= 0)
			{
				break;
			}
		}
		for (TNetUser& user : userlist)
		{
			if (FD_ISSET(user.m_Sock, &rSet))
			{
				int iRet= RecvUser(user);
				if (iRet <= 0)
				{
					user.m_bConnect = false;					
				}
			}
		}
		for (TNetUser& user : userlist)
		{
			if (FD_ISSET(user.m_Sock, &wSet))
			{
				if (user.m_packetPool.size() > 0)
				{
					std::list<TPacket>::iterator iter;
					for (iter = user.m_packetPool.begin();
						iter != user.m_packetPool.end();	)
					{
						for (TNetUser& senduser : userlist)
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
			}
		}
	}	
	net.CloseNetwork();
}