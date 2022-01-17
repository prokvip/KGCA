#pragma once
#include "TNetUser.h"
class TServer : public TServerObj
{
public:
	std::list<TNetUser*> m_UserList;
	SOCKET				 m_ListenSock;	
public:
	int SendMsg(SOCKET sock, char* msg, WORD type);
	int SendMsg(SOCKET sock, UPACKET& packet);
	int Broadcast(TNetUser* user);
	virtual bool AddUser(SOCKET socr, SOCKADDR_IN clientAddr);
public:
	virtual bool	InitServer(int iPort);
	virtual bool	Run();
	virtual bool	Release();
};

