#include "TServer.h"
#include "TDebugString.h"
#include "TServerIOCP.h"
#include "TLock.h"
void TServer::SyncCharacters(T_PACKET& pSendUser)
{
	string msg(pSendUser.packet.msg, pSendUser.packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	TCharacter tChar;
	RecvStream >> tChar;
	m_PCList.SyncCharacters(tChar);
	SyncAllCharacters(pSendUser.pUser);
}
void TServer::MoveAction(T_PACKET& pSendUser)
{
	string msg(pSendUser.packet.msg, pSendUser.packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	TCharacter tChar;
	RecvStream >> tChar;
	m_PCList.MoveTo(tChar);
	SendBroadcastPool(pSendUser.packet);
}
void TServer::CreateCharacter(T_PACKET& pSendUser)
{
	tGUID uuid;
	RPC_STATUS ret_val = ::UuidCreate(&uuid);

	string msg(pSendUser.packet.msg, pSendUser.packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	int type;
	string name;
	RecvStream >> type;
	RecvStream >> name;

	stringstream data;
	data << true ;
	data << uuid;
	data << type;
	data << name << endl;
	SendPacket(pSendUser.pUser, MakePacket(PACKET_CREATE_ACCOUNT_ACK, data));
}

void TServer::SpawnCharacters(T_PACKET& pSendUser)
{
	TCharacter tChar;
	string msg(pSendUser.packet.msg, pSendUser.packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);	
		
	RecvStream >> tChar;
	m_PCList.SpawnCharacters(tChar);

	stringstream data;
	data << tChar;
	SendBroadcastPool(MakePacket(PACKET_SPAWN_CHARACTER, data));
	//Broadcast(MakePacket(PACKET_SPAWN_CHARACTER, data));
	T_LOG("PACKET_SPAWN_CHARACTER\r\n");
}
void TServer::SyncAllCharacters(TUser* pUser)
{	
	stringstream data;
	data << m_PCList;
	SendPacket(pUser, MakePacket(PACKET_ZONE_PLAYERS_INFO, data));
}
void TServer::SendDeadPlayer(TUser* pUser)
{
	stringstream data;
	data << pUser->m_GuidActiveCharacter;
	SendPacket(pUser, MakePacket(PACKET_DEAD_CHARACTER, data));	
	m_PCList.Get()[pUser->m_GuidActiveCharacter].SetAlive(false);
}

void TServer::DeleteCharacter(T_PACKET& pSendUser) {}
void TServer::HitCharacter(T_PACKET& pSendUser) {}
void TServer::HitMonster(T_PACKET& pSendUser) {}
void TServer::AttackCharacter(T_PACKET& pSendUser) {}
void TServer::AttackMonster(T_PACKET& pSendUser) {}
void TServer::DeadCharacters(T_PACKET& pSendUser) {}
void TServer::DeadMonster(T_PACKET& pSendUser) {}
void TServer::DamageCharacter(T_PACKET& pSendUser) {}
void TServer::DamageMonster(T_PACKET& pSendUser) {}

