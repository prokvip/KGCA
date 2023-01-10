#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define NETWORK_MSG WM_USER+80
#include "TStd.h"
#include "TProtocol.h"
class TNetwork
{
public:
	bool	m_bConnect=false;
	SOCKET	m_Sock;
	HANDLE	m_hClientThread;
	std::list<UPACKET>  m_RecvPacketList;
	std::list<UPACKET>  m_SendPacketList;
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

