#include "TServer.h"
#include "TDebugString.h"
#include "TServerIOCP.h"
#include "TLock.h"
void TServer::ReqZoneEntry(T_PACKET& pSendUser)
{
	string msg(pSendUser.packet.msg, pSendUser.packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	TCharacter tChar;
	RecvStream >> tChar;

	if (m_PCList.ReqZoneEntry(tChar))
	{
		pSendUser.pUser->m_iType = 1;
	}

	AckZoneEntry(pSendUser, tChar);
	MapInfoSC(pSendUser);
	BroadcastNewPlayer(tChar);
	pSendUser.pUser->m_ConnectType = CONNECT_TYPE::C_GAMEPLAYING;
	T_LOG("PACKET_ZONE_ENTRY_REQ\r\n");
}
void TServer::AckZoneEntry(T_PACKET& pSendUser, TCharacter& tChar)
{
	m_PCList.AckZoneEntry(tChar);
	stringstream data;
	data << m_PCList;
	data << m_NpcMgr;
	SendPacket(pSendUser.pUser, MakePacket(PACKET_ZONE_ENTRY_ACK, data));	
	T_LOG("UserCnt=%d, TargetCnt = %d, NpcCnt=%d \r\n", m_PCList.Get().size(), m_tpMgr.Get().size(),  m_NpcMgr.GetNpcCount());
}

void TServer::MapInfoSC(T_PACKET& pSendUser)
{
	if (pSendUser.pUser->m_iType == 1 && m_tpMgr.Get().size() <= 0)
	{
		stringstream data;
		SendPacket(pSendUser.pUser, MakePacket(PACKET_ZONE_TARGET_REQ, data));
		T_LOG("PACKET_ZONE_TARGET_REQ\r\n");
	}	
}


void TServer::RecvZoneTargetPoint(T_PACKET& pSendUser)
{
	string msg(pSendUser.packet.msg, pSendUser.packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	int iNumCount = 0;
	tBlockRead<int>(RecvStream, iNumCount);

	m_tpMgr.Get().clear();
	m_NpcMgr.Release();

	TPACKET_TARGETPOINT_INFO tp;
	for (int i = 0; i < iNumCount; i++)
	{
		tBlockRead<TPACKET_TARGETPOINT_INFO>(RecvStream, tp);
		m_tpMgr.Get()[i] = tp;
		m_NpcMgr.Add(tp.vPosition);
	}
	SpawnMonsterList();	
	T_LOG("[%s:%d]\r\n", "NPC_LIST", m_NpcMgr.GetNpcCount());
	T_LOG("PACKET_ZONE_TARGET_ACK\r\n");
}