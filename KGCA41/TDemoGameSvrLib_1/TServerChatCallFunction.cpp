#include "TServer.h"
#include "TDebugString.h"
#include "TServerIOCP.h"
#include "TLock.h"

void TServer::Msg(T_PACKET& pSendUser)
{
	ISessionMgr.Lock();
		UPACKET* pPacket = &(pSendUser.packet);
		string msg(pSendUser.packet.msg, pSendUser.packet.ph.len - PACKET_HEADER_SIZE);
		stringstream RecvStream(msg);
		tGUID tGuid;
		string recvMsg;

		RecvStream >> tGuid;
		RecvStream >> recvMsg;

		T_LOG("Message:%s\r\n", recvMsg.c_str());

		for (TUserIterator iter = GetUserList().begin();iter != GetUserList().end();iter++)
		{
			TUser* pUser = (TUser*)iter->second;
			if (SendPacket(pUser, pSendUser.packet) <= 0)
			{
				break;
			}
		}
	ISessionMgr.Unlock();
}
void TServer::ReqChatName(T_PACKET& pSendUser) {}
void TServer::AckChatName(T_PACKET& pSendUser)
{
	UPACKET* pPacket = &(pSendUser.packet);

	UPACKET PacketMsg;
	ZeroMemory(&PacketMsg, sizeof(PacketMsg));
	PacketMsg.ph.iotype = 777;
	PacketMsg.ph.len = pPacket->ph.len;
	PacketMsg.ph.type = PACKET_CHAT_MSG;

	pPacket->msg[pPacket->ph.len - PACKET_HEADER_SIZE] = 0;
	C_STR name = pPacket->msg;
	memcpy(&PacketMsg.msg, &pPacket->msg, pPacket->ph.len - PACKET_HEADER_SIZE);

	char buffer[64] = "님이 입장하였습니다.";
	memcpy(&PacketMsg.msg[pPacket->ph.len - PACKET_HEADER_SIZE], buffer, strlen(buffer));
	PacketMsg.ph.len += (uint16_t)strlen(buffer);

	T_LOG("Message:%s\r\n", PacketMsg.msg);
	SendBroadcastPool(PacketMsg);
	//Broadcast(PacketMsg);
}