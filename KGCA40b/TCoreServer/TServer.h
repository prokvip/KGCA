#pragma once
#include "TNetUser.h"
class TServer
{
public:
	std::list<TNetUser> g_UserList;
	CRITICAL_SECTION g_CS;
	HANDLE g_hMutex;
	SOCKET m_ListenSock;
public:
	int SendMsg(SOCKET sock, char* msg, WORD type);
	int SendMsg(SOCKET sock, UPACKET& packet);
	int Broadcast(TNetUser& user);
	int RecvUser(TNetUser& user);
public:
	bool		InitServer(int iPort);
	bool		Run();
	bool		Release();
};

