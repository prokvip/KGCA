#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "TStd.h"
#include "TProtocol.h"
struct TUser
{
	SOCKET sock;
	SOCKADDR_IN address;
	char m_szName[9] = { 0, };
	char szRecvMsg[255] = { 0, };
	int iTotalRecvBytes = 0;
};
struct TPacket
{
	TUser* pUser;
	UPACKET packet;
};

class TNetwork
{
public:
	FD_SET		m_rSet;
	FD_SET		m_wSet;
	std::list<TUser> userlist;
public:
	bool	m_bConnect=false;
	SOCKET	m_Sock;
	std::list<UPACKET>  m_RecvPacketList;
	std::list<std::pair<SOCKET, UPACKET>>  m_SendPacketList;
	std::list<UPACKET>  m_BroadcasttingPacketList;
public:
	typedef std::function<void(UPACKET& t)> CallFuction;
	typedef std::map<DWORD, CallFuction>::iterator FunIter;
	std::map<DWORD, CallFuction> m_fnExecutePacket;
public:
	void   PacketProcess();
	void   MakePacket(UPACKET& packet, const char* msg, int iSize, short type);
	bool   NetStart(std::string ip, int iPort);
	void   AddSend(SOCKET sock, const char* msg, int iSize, short type);
	int    SendMsg(SOCKET sock, const char* msg, int iSize, short type);
	void   SendPrecess();
	void   RecvPrecess();
	bool   Frame();
	bool   Render();
	bool   Release();
public:
	TNetwork();
	virtual ~TNetwork();
};

