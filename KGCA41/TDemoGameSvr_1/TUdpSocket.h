#pragma once
#include "TServerstd.h"
#include <ws2tcpip.h>

class TUdpSocket: public TThread
{
public:
	SOCKET		m_Socket;
	SOCKADDR_IN m_Remoteaddr;
	SOCKADDR_IN m_RecvAddr;
	ip_mreq		m_MemberReq;
public:
	bool		Init();
	bool		Run();
	bool		Release();
public:
	TUdpSocket(void);
	~TUdpSocket(void);
};
