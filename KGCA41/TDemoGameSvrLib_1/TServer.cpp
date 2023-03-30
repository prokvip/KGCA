#include "TServer.h"
#include "TDebugString.h"
#include "TServerIOCP.h"
#include "TLock.h"

TServer*	TServer::GetPtr()
{
	return this;
}
TAcceptor& TServer::GetAcceptor()
{
	return m_Acceptor;
}
TTargetPointMgr& TServer::GetTargetPointMgr()
{
	return m_tpMgr;
}

TUserList&  TServer::GetUserList()
{
	return ISessionMgr.GetSessionList();
}
TPlayerCharacter&	TServer::GetPCList()
{
	return m_PCList;
}
void TServer::PrintGuid(tGUID& guid, const char* msg)
{
	string guidString;
	RPC_CSTR szUuid = NULL;
	if (::UuidToStringA(&guid, &szUuid) == RPC_S_OK)
	{
		guidString = (char*)szUuid;
		::RpcStringFreeA(&szUuid);
	}
	//T_LOG("%s=%s\r\n", guidString.c_str(), msg);

}
void TServer::BroadcastNewPlayer(TCharacter& player)
{
	stringstream data;
	data << player;
	Broadcast(MakePacket(PACKET_ZONE_NEW_PLAYER, data));
	T_LOG("PACKET_ZONE_NEW_PLAYER Send\r\n");
}

void TServer::Broadcast(UPACKET& packet)
{
	SendBroadcastPool(packet);
	/*ISessionMgr.Lock();
		for (TUserIterator iter = GetUserList().begin();
			iter != GetUserList().end();
			iter++)
		{
			TUser* pUser = (TUser*)iter->second;
			if (SendPacket(pUser, packet) <= 0)
			{
				continue;
			}
		}
	ISessionMgr.Unlock();*/
}
void TServer::Broadcast(T_PACKET& tPacket)
{
	SendBroadcastPool(tPacket.packet);
	/*ISessionMgr.Lock();
		for (TUserIterator iter = GetUserList().begin();iter != GetUserList().end();iter++)
		{
			TUser* pUser = (TUser*)iter->second;
			if (pUser->m_Socket != tPacket.pUser->m_Socket)
			{
				if (SendPacket(pUser, tPacket.packet) <= 0)
				{
					continue;
				}
			}
		}
	ISessionMgr.Unlock();*/
}
void TServer::Broadcast(stringstream & SendStream)
{
	SendBroadcastPool(SendStream);
	/*ISessionMgr.Lock();
		for (auto user : GetUserList())
		{
			CopyMemory(user.second->m_sendBuffer, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
			SendPacket(user.second, SendStream.str().length());
		}
	ISessionMgr.Unlock();*/
}
void TServer::Broadcast(tGUID tGuidExcept, stringstream & SendStream)
{
	SendBroadcastPool(SendStream);
	/*ISessionMgr.Lock();
		for (auto user : GetUserList())
		{
			if (tGuidExcept != user.second->m_GuidActiveCharacter)
			{
				CopyMemory(user.second->m_sendBuffer, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
				SendPacket(user.second, SendStream.str().length());
			}
		}
	ISessionMgr.Unlock();*/
}
int TServer::SendPacket(TUser* pUser, stringstream & SendStream)
{	
	T_PACKET Packet;
	CopyMemory(&Packet.packet, (char*)SendStream.str().c_str(), SendStream.str().length());
	Packet.pUser = pUser;
	Packet.sock = pUser->m_Socket;
	m_SendPacketPool.AddPacket(Packet);
	return 1;
}
void TServer::SendBroadcastPool(UPACKET& sendmsg)
{
	T_PACKET Packet;
	CopyMemory(&Packet.packet, (char*)&sendmsg, sendmsg.ph.len);
	m_SendBroadcastPool.AddPacket(Packet);
}
void TServer::SendBroadcastPool(stringstream & SendStream)
{
	UPACKET packet;
	CopyMemory(&packet, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
	SendBroadcastPool(packet);
}
void TServer::SendBroadcastPacket()
{
	ISessionMgr.Lock();
	m_SendBroadcastPool.Lock();

	for (TPacketIterator packet = m_SendBroadcastPool.Get().begin();
		packet != m_SendBroadcastPool.Get().end();
		packet++)
	{		
		T_PACKET tPacket = (*packet);
		for (TUserIterator iter = GetUserList().begin();
			iter != GetUserList().end();
			iter++)
		{
			TUser* pUser = (TUser*)iter->second;
			if (SendPacket(pUser, tPacket.packet) <= 0)
			{
				continue;
			}
		}		
	}
	m_SendBroadcastPool.Clear();
	m_SendBroadcastPool.Unlock();
	ISessionMgr.Unlock();
}
int TServer::SendPacket(TUser* pUser, UPACKET& sendmsg)
{
	T_PACKET Packet;	
	CopyMemory(&Packet.packet, (char*)&sendmsg, sendmsg.ph.len);
	Packet.pUser = pUser;
	Packet.sock = pUser->m_Socket;
	m_SendPacketPool.AddPacket(Packet);
	return 1;
}
void TServer::SendPacket(TUser * pUser, size_t iSendByte)
{
	T_PACKET Packet;
	CopyMemory(&Packet.packet, (char*)&pUser->m_sendBuffer, iSendByte);
	Packet.pUser = pUser;
	Packet.sock = pUser->m_Socket;
	m_SendPacketPool.AddPacket(Packet);
}
int TServer::DirectSendPacket(TUser* pUser, stringstream & SendStream)
{
	CopyMemory(pUser->m_sendBuffer, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
	pUser->m_wsaSendBuffer.buf = pUser->m_sendBuffer;
	pUser->m_wsaSendBuffer.len = (ULONG)SendStream.str().length();
	OVERLAPPED2* pOverlapped = pUser->Overlapped(OVERLAPPED2::MODE_SEND);
	//OVERLAPPED2& pOverlapped = pUser->Overlapped(OVERLAPPED2::MODE_SEND);
	DWORD dwSendByte = (DWORD)SendStream.str().length();

	T_PACKET packet;
	CopyMemory(&packet, (char*)SendStream.str().c_str(), SendStream.str().length());

	int iRet = WSASend(pUser->m_Socket, &pUser->m_wsaSendBuffer, 1, NULL, 0, 
		(LPOVERLAPPED)pOverlapped, NULL);
	if (iRet == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		////I_DebugStr.T_ERROR();
		return 0;
	}
	return 1;
}
int TServer::DirectSendPacket(TUser* pUser, UPACKET& sendmsg)
{
	pUser->m_wsaSendBuffer.buf = (char*)&sendmsg;
	pUser->m_wsaSendBuffer.len = sendmsg.ph.len;
	OVERLAPPED2* pOverlapped = pUser->Overlapped(OVERLAPPED2::MODE_SEND);
	//OVERLAPPED2& pOverlapped = pUser->Overlapped(OVERLAPPED2::MODE_SEND);
	DWORD dwSendByte = sendmsg.ph.len;

	int iRet = WSASend(pUser->m_Socket,
		&pUser->m_wsaSendBuffer,
		1,
		NULL,
		0,
		(LPOVERLAPPED)pOverlapped,
		NULL);

	if (iRet == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		////I_DebugStr.T_ERROR();
		return 0;
	}
	return 1;
}
void TServer::DirectSendPacket(TUser * pUser, size_t iSendByte)
{
	DWORD dwFlags = 0;
	pUser->m_wsaSendBuffer.buf = pUser->m_sendBuffer;
	pUser->m_wsaSendBuffer.len = (ULONG)iSendByte;
	OVERLAPPED2* pOverlapped = pUser->Overlapped(OVERLAPPED2::MODE_SEND);
	//OVERLAPPED2& pOverlapped = pUser->Overlapped(OVERLAPPED2::MODE_SEND);
	DWORD dwSendByte = (DWORD)iSendByte;

	if (pUser->m_Socket == INVALID_SOCKET) return;
	int iRet = WSASend(pUser->m_Socket, &(pUser->m_wsaSendBuffer), 1, NULL, dwFlags, 
		(LPOVERLAPPED)pOverlapped, NULL);
	if (iRet == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		////I_DebugStr.T_ERROR();
	}
}
bool TServer::Init()
{	
	WSADATA wsa;
	if( WSAStartup( MAKEWORD(2,2), &wsa ) != 0 )
	{
		return false;
	}

	OVERLAPPED2::prepareAllocation();

	m_RecvPacketPool.m_iType = 1;
	m_SendPacketPool.m_iType = 2;

	m_fnExecutePacket[PACKET_VERIFICATION_REQ] = &TServer::ReqVerification;	
	m_fnExecutePacket[PACKET_CHAT_NAME_REQ] = &TServer::ReqChatName;
	m_fnExecutePacket[PACKET_CHAT_NAME_ACK] = &TServer::AckChatName;
	m_fnExecutePacket[PACKET_CHAT_MSG]		= &TServer::Msg;
	m_fnExecutePacket[PACKET_USER_POSITION] = &TServer::MoveAction;
	m_fnExecutePacket[PACKET_STOP_POSITION] = &TServer::MoveAction;
	
	m_fnExecutePacket[PACKET_LOGIN_REQ]			= &TServer::Login;
	m_fnExecutePacket[PACKET_LOGOUT_REQ]		= &TServer::Logout;
	m_fnExecutePacket[PACKET_CREATE_ACCOUNT_REQ] = &TServer::CreateAccount;
	m_fnExecutePacket[PACKET_DELETE_ACCOUNT_REQ] = &TServer::DeleteAccount;
	m_fnExecutePacket[PACKET_CREATE_CHARACTER_REQ] = &TServer::CreateCharacter;
	m_fnExecutePacket[PACKET_DELETE_CHARACTER_REQ] = &TServer::DeleteCharacter;
	m_fnExecutePacket[PACKET_ZONE_ENTRY_REQ] = &TServer::ReqZoneEntry;
	m_fnExecutePacket[PACKET_ZONE_TARGET_ACK] = &TServer::RecvZoneTargetPoint;

	m_fnExecutePacket[PACKET_HIT_CHARACTER] = &TServer::HitCharacter;
	m_fnExecutePacket[PACKET_HIT_NPC] = &TServer::HitMonster;
	m_fnExecutePacket[PACKET_ATTACK_CHARACTER] = &TServer::AttackCharacter;
	m_fnExecutePacket[PACKET_ATTACK_NPC] = &TServer::AttackMonster;
	m_fnExecutePacket[PACKET_DEAD_CHARACTER] = &TServer::DeadCharacters;
	m_fnExecutePacket[PACKET_DEAD_NPC] = &TServer::DeadMonster;
	m_fnExecutePacket[PACKET_DAMAGE_CHARACTER] = &TServer::DamageCharacter;
	m_fnExecutePacket[PACKET_DAMAGE_NPC] = &TServer::DamageMonster;
	m_fnExecutePacket[PACKET_SPAWN_CHARACTER] = &TServer::SpawnCharacters;
	m_fnExecutePacket[PACKET_SPAWN_NPC] = &TServer::SpawnMonster;
	m_fnExecutePacket[PACKET_SYNC_CHARACTER] = &TServer::SyncCharacters;
	m_fnExecutePacket[PACKET_SYNC_NPC_LIST] = &TServer::SyncMonster;
	
	// IOCP 생성
	I_ServerIOCP.Init();
	
	
	// Accept 처리
	if( !m_Acceptor.Set(10000) )
	{		
		return false;
	}

	m_NpcMgr.Set();

	CreateThread();

	
	return true;
}

bool TServer::Frame()
{
	for (auto & player : GetPCList().Get())
	{
		TCharacter& playCharacter = player.second;
		if (!playCharacter.GetAlive())
		{
			continue;
		}
		if (playCharacter.GetPos().DistanceTo(playCharacter.GetTarget())
			> 300)
		{
			FVector vPos = playCharacter.GetPos();
			FVector vDir = playCharacter.GetTarget() - vPos;
			vDir.Normalize(0.0f);
			vPos += vDir * playCharacter.GetVelocity();
			playCharacter.SetPos(vPos);
		}
	}
	return true;
}
bool TServer::Render()
{
	return true;
}
bool TServer::Release()
{	
	ISessionMgr.ReleaseAll();
	m_PCList.ReleaseAll();
	WSACleanup();		
	return true;
}
void  TServer::AddRecvPacket(T_PACKET pack)
{
	m_RecvPacketPool.AddPacket(pack);
}
bool TServer::Run()
{
	while (m_bStarted)
	{
		// Recv
		{
			m_RecvPacketPool.Lock();			
			for (TPacketIterator iter = m_RecvPacketPool.Get().begin();
				iter != m_RecvPacketPool.Get().end();
				iter++)
			{
				UPACKET PacketMsg;
				ZeroMemory(&PacketMsg, sizeof(PacketMsg));
				T_PACKET pSendUser = (*iter);
				FunctionIterator calliter = m_fnExecutePacket.find(pSendUser.packet.ph.type);
				if (calliter != m_fnExecutePacket.end())
				{
					CallFuction call = calliter->second;
					(this->*call)(*iter);
					//T_LOG("%s%d[%d]\r\n", "TYPE=", pSendUser.packet.ph.type, pSendUser.packet.ph.len );
				}
			}
			
			InterlockedExchangeAdd(&m_dwRecvPacket, m_RecvPacketPool.Get().size());
			m_RecvPacketPool.Clear();
			m_RecvPacketPool.Unlock();			
		}

		SendBroadcastPacket();

		// Send
		{
			m_SendPacketPool.Lock();		

			for (TPacketIterator iter = m_SendPacketPool.Get().begin();
				iter != m_SendPacketPool.Get().end();
				iter++)
			{
				T_PACKET tPacket = (*iter);				
				tPacket.ov = new OVERLAPPED2(OVERLAPPED2::MODE_SEND);
				tPacket.buffer.buf = (char*)&tPacket.packet;
				tPacket.buffer.len = (ULONG)tPacket.packet.ph.len;
				DWORD dwFlags = 0;
				int iRet = WSASend(tPacket.sock, &tPacket.buffer, 1, NULL, dwFlags, 
					(LPOVERLAPPED)tPacket.ov, NULL);
				if (iRet == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
				{
					delete tPacket.ov;
					I_Server.DelUser(tPacket.pUser->m_Socket);
				}
			}
			InterlockedExchangeAdd(&m_dwSendPacket, m_SendPacketPool.Get().size());
			m_SendPacketPool.Clear();
			m_SendPacketPool.Unlock();		
		}
		Sleep(1);
	}
	return true;
}
TUser* TServer::GetUser(SOCKET iIndex )
{
	return ISessionMgr.Find(iIndex);
}
void TServer::ChangeSuperUser(tGUID tGuid)
{
	m_PCList.ChangeSuperUser(tGuid);
}
bool TServer::DelUser( SOCKET sock)
{
	if (GetUserList().size() <= 0) return true;

	tGUID tUserGuid, tActiveCharGuid;
	UuidCreateNil(&tUserGuid);
	UuidCreateNil(&tActiveCharGuid);
	// iIndex에 해당하는 tUserGuid, tCharGuid를 얻는다.
	if ( ISessionMgr.DelUser(sock, tUserGuid, tActiveCharGuid) )
	{
		ChangeSuperUser(tActiveCharGuid);
		m_PCList.Delete(tActiveCharGuid);
		SendLogoutPlayer(tActiveCharGuid);
		T_LOG("%s%d\r\n", "*User Count=", GetUserList().size());
		T_LOG("%s%d\r\n", "*Char Count=", m_PCList.Get().size());
		return true;
	}	
	return false;
}

TServer::TServer(void)
{
	m_dwSendPacket = 0;
	m_dwRecvPacket = 0;
}
TServer::~TServer(void)
{
	ObjectPool<OVERLAPPED2>::AllFree();
}

ostream& operator<<(ostream &stream, TServer& info)
{
	ISessionMgr.Lock();
		stream << info.GetUserList().size();
		for (auto& user : info.GetUserList())
		{
			stream << user.first;
			stream << *user.second;
		}
	ISessionMgr.Unlock();
	return stream;
}

istream &operator>>(istream &stream, TServer& info)
{
	ISessionMgr.Lock();
		size_t iNumUsers = 0;
		tGUID guid;
		TUser* pUser = nullptr;
		SAFE_NEW(pUser, TUser);
		info.GetUserList().clear();

		stream >> iNumUsers;
		for (int i = 0; i < iNumUsers; i++)
		{
			stream >> guid;
			stream >> *pUser;
			info.GetUserList()[guid] = pUser;
		}
	ISessionMgr.Unlock();
	return stream;
}