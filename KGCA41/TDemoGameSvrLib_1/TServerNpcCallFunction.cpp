#include "TServer.h"
#include "TDebugString.h"
#include "TServerIOCP.h"
#include "TLock.h"

void TServer::SpawnMonster(T_PACKET& pSendUser) 
{
	string msg(pSendUser.packet.msg, pSendUser.packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	TCharacter tChar;
	tBlockRead<TCharacter>(RecvStream, tChar);
	m_NpcMgr.Add(tChar.GetPos());
	SpawnMonsterList();	
}
void TServer::SyncMonster(T_PACKET& pSendUser)
{
	string msg(pSendUser.packet.msg, pSendUser.packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	RecvStream >> m_NpcMgr;
}
void TServer::SpawnMonsterList()
{
	stringstream data;
	data << m_NpcMgr;
	SendBroadcastPool(MakePacket(PACKET_SPAWN_NPC_LIST, data));
	//Broadcast(MakePacket(PACKET_SPAWN_NPC_LIST, data));
}