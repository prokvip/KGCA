#pragma once
#include "TNetUser.h"

class TNetwork
{
public:
	SOCKET  m_Sock;
	std::list<TNetUser> userlist;
	TNetUser   m_PlayerUser;
public:
	bool	InitNetwork();
	bool	CloseNetwork();	
	bool	InitServer(int protocol, int iport, const char* ip=nullptr);
public:
	int SendMsg(SOCKET sock, char* msg, WORD type);
	int SendMsg(SOCKET sock, UPACKET& packet);
	int AddUser(SOCKET sock);
	int RecvUser(TNetUser& user);
};

