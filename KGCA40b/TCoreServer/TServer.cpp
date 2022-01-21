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
	
	m_Accept.Set(iPort);
	m_Accept.Create(this);
	m_Accept.Detach();
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
bool TServer::DelUser(SOCKET sock)
{
	return true;
}
bool TServer::DelUser(TNetUser* pUser)
{
	pUser->DisConnect();
	return true;
}
bool TServer::DelUser(m_UserIter& iter)
{
	DelUser((TNetUser*)*iter);
	return true;
}
bool TServer::Release()
{	
	TObjectPool<TNetUser>::AllFree();
	TObjectPool<TOV>::AllFree();
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