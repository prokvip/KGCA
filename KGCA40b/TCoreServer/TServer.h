#pragma once
#include "TNetUser.h"

struct XPacket 
{
	TNetUser* pUser;	
	TPacket   packet;
};

class TServer : public TServerObj
{
public:
	std::list<TNetUser*> m_UserList;
	SOCKET				 m_ListenSock;	
	std::list<XPacket>	 m_packetPool;
	typedef std::list<TNetUser*>::iterator m_UserIter;
public:
	int SendMsg(SOCKET sock, UPACKET& packet);
	int SendMsg(TNetUser* pUser, char* msg, int iSize,  WORD type);
	int SendMsg(TNetUser* pUser, UPACKET& packet);
	int Broadcast(TNetUser* user);
	virtual bool AddUser(SOCKET socr, SOCKADDR_IN clientAddr);
public:
	virtual bool	InitServer(int iPort);
	virtual bool	Run();
	virtual bool	Release();
};

