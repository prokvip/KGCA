#pragma once
#include "TServer.h"
#include "TChatUser.h"
#define MAX_WORKER_THREAD 3
class TLobbyServer : public TServer
{
public:
	HANDLE g_hWorkThread[MAX_WORKER_THREAD];
	HANDLE g_hIOCP;
public:
	virtual void LoginReq(TPacket& t, TNetUser* user) override;
	virtual void ChatMsg(TPacket& t, TNetUser* user);
public:
	virtual bool	InitServer(int iPort);
	virtual bool	Run();
	virtual bool	Release();
	virtual bool	AddUser(SOCKET socr, SOCKADDR_IN clientAddr);
};

