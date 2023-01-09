#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "TStd.h"
#include "TProtocol.h"
class TNetwork
{
public:
	SOCKET m_Sock;
	HANDLE m_hClientThread;
	std::list<UPACKET>  m_PacketList;
public:
	bool   NetStart(std::string ip, int iPort);
	int    SendMsg(SOCKET sock, const char* msg, short type);
	bool   Frame();
	bool   Render();
	bool   Release();
public:
	TNetwork();
	virtual ~TNetwork();
};

