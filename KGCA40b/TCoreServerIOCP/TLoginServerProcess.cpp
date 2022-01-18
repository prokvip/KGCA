#include "TLobbyServer.h"
bool TLobbyServer::Run()
{
	while (1)
	{
		EnterCriticalSection(&m_cs);
		for (TNetUser* tUser : m_UserList)
		{
			TChatUser* pChat = (TChatUser*)tUser;
			if (pChat->m_packetPool.size() > 0)
			{
				// tUser->Process();				
			}
		}

		// ��Ŷó��
		std::list<XPacket>::iterator iter;
		for (iter = m_packetPool.begin();
			iter != m_packetPool.end();)
		{
			XPacket* xp = (XPacket*)&(*iter);
			switch (xp->packet.m_uPacket.ph.type)
			{
			case PACKET_LOGIN_REQ:
			{
				TLoginReq login;
				memcpy(&login, xp->packet.m_uPacket.msg, sizeof(TLoginReq));
				// DB ���ιް� ��ȯ
				TLoginAck ack;
				ack.iResult = 1;
				SendMsg(xp->pUser,
					(char*)&ack,
					sizeof(TLoginAck),
					PACKET_LOGIN_ACK);
			}break;
			case PACKET_CHAT_MSG:
			{
			}break;
			case PACKET_CHAT_NAME_REQ: {}break;
			case PACKET_CHAT_NAME_ACK: {}break;
			case PACKET_LOGOUT_REQ: {}break;
			case PACKET_LOGOUT_ACK: {}break;
			case PACKET_LOGOUT_PLAYER: {}break;
			}
			iter = m_packetPool.erase(iter);
		}
		// �ֱ����� ����ȭ
		for (TNetUser* tUser : m_UserList)
		{
			TChatUser* pChat = (TChatUser*)tUser;
			if (pChat->m_packetPool.size() > 0)
			{
				Broadcast(tUser);
			}
		}


		for (m_UserIter iter = m_UserList.begin();
			iter != m_UserList.end();)
		{
			if ((*iter)->m_bConnect == false)
			{
				(*iter)->DisConnect();
				delete (*iter);
				iter = m_UserList.erase(iter);
			}
			else
			{
				iter++;
			}
		}

		LeaveCriticalSection(&m_cs);
		Sleep(1);
	}
	return true;
}