#pragma once
#include "TSessionMgr.h"

class TServer: public TSingleton<TServer>, public TThread
{
private:
	friend class TSingleton<TServer>;
private:
	TPacketPool			m_SendBroadcastPool;
	TPacketPool			m_RecvPacketPool;
	TPacketPool			m_SendPacketPool;
	TAcceptor			m_Acceptor;
	TNpcManager			m_NpcMgr;
	TPlayerCharacter	m_PCList; // active
	TTargetPointMgr		m_tpMgr;
	typedef void(TServer::*CallFuction)(T_PACKET& t);
	std::map<int, CallFuction>				m_fnExecutePacket;
	typedef std::map<int, CallFuction>::iterator FunctionIterator;
public:
	volatile DWORD 		m_dwSendPacket;
	volatile DWORD 		m_dwRecvPacket;
	TAcceptor&			GetAcceptor();
	TTargetPointMgr&	GetTargetPointMgr();
	TUserList&			GetUserList();
	TPlayerCharacter&	GetPCList(); // active
	TServer*			GetPtr();
public:
	virtual bool		Init();
	virtual bool		Run();
	virtual bool		Frame();
	
	virtual bool		Render();
	virtual bool		Release();
	virtual void		AddRecvPacket(T_PACKET pack);
	virtual int			SendPacket(TUser* pUser, UPACKET& PacketMsg);
	virtual int			SendPacket(TUser* pUser, stringstream & SendStream);
	void				SendPacket(TUser * pUser, size_t iSendByte);
	virtual int			DirectSendPacket(TUser* pUser, UPACKET& PacketMsg);
	virtual int			DirectSendPacket(TUser* pUser, stringstream & SendStream);
	void				DirectSendPacket(TUser * pUser, size_t iSendByte);
	// broadcast pool save -> run ->send
	void				SendBroadcastPool(UPACKET& sendmsg);
	void				SendBroadcastPool(stringstream & SendStream);
	void				SendBroadcastPacket();
	virtual void		Broadcast(T_PACKET& pSendUser);
	virtual void		Broadcast(UPACKET& pSendUser);
	void				Broadcast(stringstream & SendStream);
	void				Broadcast(tGUID tGuidExcept, stringstream & SendStream);
public:
	virtual TUser*		GetUser(SOCKET iIndex );
	virtual bool		DelUser(SOCKET iIndex );
	//virtual bool		DelUser(tGUID tGuid);
	virtual void		ReqVerification(T_PACKET& t);
	virtual void		BroadcastNewPlayer(TCharacter& player);
	virtual void		SyncAllCharacters(TUser* pUser);
	virtual void		SendDeadPlayer(TUser* pUser);
	virtual void        SendLogoutPlayer(tGUID tGuid);
	virtual void		ChangeSuperUser(tGUID tGuid);
public:
	virtual void		MoveAction(T_PACKET& t);
	virtual void		Msg(T_PACKET& t);
	virtual void		AckChatName(T_PACKET& t);
	virtual void		ReqChatName(T_PACKET& t);
	virtual void		CreateAccount(T_PACKET& pSendUser);
	virtual void		DeleteAccount(T_PACKET& pSendUser);
	virtual void		Login(T_PACKET& pSendUser);
	virtual void		Logout(T_PACKET& pSendUser);
	virtual void		CreateCharacter(T_PACKET& pSendUser);
	virtual void		DeleteCharacter(T_PACKET& pSendUser);
	virtual void		HitCharacter(T_PACKET& pSendUser);
	virtual void		HitMonster(T_PACKET& pSendUser);
	virtual void		ReqZoneEntry(T_PACKET& pSendUser);
	virtual void		AckZoneEntry(T_PACKET& pSendUser, TCharacter& tChar);
	virtual void		RecvZoneTargetPoint(T_PACKET& pSendUser);
	virtual void		MapInfoSC(T_PACKET& pSendUser);
	virtual void		AttackCharacter(T_PACKET& pSendUser);
	virtual void		AttackMonster(T_PACKET& pSendUser);
	virtual void		DeadCharacters(T_PACKET& pSendUser);
	virtual void		DeadMonster(T_PACKET& pSendUser);
	virtual void		DamageCharacter(T_PACKET& pSendUser);
	virtual void		DamageMonster(T_PACKET& pSendUser);
	virtual void		SpawnCharacters(T_PACKET& pSendUser);
	virtual void		SpawnMonster(T_PACKET& pSendUser);
	virtual void		SyncCharacters(T_PACKET& pSendUser);
	virtual void		SyncMonster(T_PACKET& pSendUser);
public:
	void SpawnMonsterList();
	void PrintGuid(tGUID& guid, const char* msg);
public:
	friend ostream& operator<<(ostream &stream, TServer& info);
	friend istream &operator>>(istream &stream, TServer& info);
public:
	TServer(void);
	virtual ~TServer(void);
};
#define I_Server TServer::GetInstance()