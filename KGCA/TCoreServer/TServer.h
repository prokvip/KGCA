#pragma once
#include "TNetUser.h"
#include "TAccepter.h"
struct XPacket 
{
	TNetUser* pUser;	
	TPacket   packet;
};

class TServer : public TServerObj
{
	TAccepter			m_Accept;
public:
	std::list<TNetUser*> m_UserList;
	std::list<XPacket>	 m_packetPool;
	typedef std::list<TNetUser*>::iterator m_UserIter;
public:
	// #include <functional>
	using CallFunction = std::function<void(TPacket& t, TNetUser* user)>;	
	typedef std::map<int, CallFunction>::iterator FuncionIterator;
	std::map<int, CallFunction> m_fnExecutePacket;
public:
	virtual void LoginReq(TPacket& t, TNetUser* user);
public:
	int SendMsg(SOCKET sock, UPACKET& packet);
	int SendMsg(TNetUser* pUser, char* msg, int iSize,  WORD type);
	int SendMsg(TNetUser* pUser, UPACKET& packet);
	int Broadcast(TPacket& t);
	int BroadcastUserPacketPool(TNetUser* user);
	virtual bool AddUser(SOCKET socr, SOCKADDR_IN clientAddr);
protected:
	virtual bool DelUser(SOCKET sock);
	virtual bool DelUser(TNetUser* pUser);
	virtual bool DelUser(m_UserIter& iter);
public:
	virtual bool	InitServer(int iPort);
	virtual bool	Run();
	virtual bool	Release();
};

