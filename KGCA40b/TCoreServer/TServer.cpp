#include "TServer.h"
void TServer::LoginReq(TPacket& t, TNetUser* user)
{

}
//void TServer::ChatMsg(TPacket& t, TNetUser* user)
//{
//
//}
bool TServer::InitServer(int iPort)
{
	m_fnExecutePacket[PACKET_LOGIN_REQ] =
		std::bind(&TServer::LoginReq, this,
			std::placeholders::_1, 
			std::placeholders::_2 );
	

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
	std::cout << "서버 가동중......." << std::endl;
	u_long on = 1;
	ioctlsocket(m_ListenSock, FIONBIO, &on);

	return true;
}
bool TServer::Run()
{	
	return true;
}
bool TServer::AddUser(SOCKET sock, SOCKADDR_IN clientAddr)
{
	return true;
}
bool TServer::Release()
{
	TObjectPool<TNetUser>::AllFree();
	TObjectPool<TOV>::AllFree();

	closesocket(m_ListenSock);
	WSACleanup();
	return true;
}
int TServer::SendMsg(SOCKET sock, UPACKET& packet)
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
int TServer::SendMsg(TNetUser* pUser, char* msg, int iSize, WORD type)
{
	// 보내는 패킷 풀에 저장하고 일괄 전송시스템.
	pUser->SendMsg(msg, iSize, type);
	return 0;
}
int TServer::SendMsg(TNetUser* pUser, UPACKET& packet)
{
	pUser->SendMsg(packet);
	return 0;
}
int TServer::BroadcastUserPacketPool(TNetUser* user)
{
	if (user->m_packetPool.size() > 0)
	{
		std::list<TPacket>::iterator iter;
		for (iter = user->m_packetPool.begin();
			iter != user->m_packetPool.end(); )
		{
			for (TNetUser* senduser : m_UserList)
			{
				int iRet = SendMsg(senduser->m_Sock, (*iter).m_uPacket);
				if (iRet <= 0)
				{
					senduser->m_bConnect = false;
				}
			}
			iter = user->m_packetPool.erase(iter);
		}
	}
	return 1;
}
int TServer::Broadcast(TPacket& t)
{
	for (TNetUser* senduser : m_UserList)
	{
		int iRet = SendMsg(senduser->m_Sock,t.m_uPacket);
		if (iRet <= 0)
		{
			senduser->m_bConnect = false;
		}
	}		
	return 1;
}