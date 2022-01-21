#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "TLobbyServer.h"
#include "TAccepter.h"
#include "TObjectPool.h"

//struct xUser : public TObjectPool<xUser>
//{
//public:
//	int   m_iIndex = 0;
//	xUser(int i)
//	{
//		m_iIndex = i;
//	}
//};

void main()
{
	//xUser* pX[20] = { nullptr, };
	//for (int i = 0; i < 8; i++)
	//{
	//	pX[i] = new xUser(i);		
	//}
	//for (int i = 0; i < 4; i++)
	//{
	//	delete pX[i];
	//}
	//for (int i = 0; i < 4; i++)
	//{
	//	pX[i] = new xUser(i+10);
	//}

	TLobbyServer server;
	server.InitServer(10000);

	server.Run();
	server.Release();
}
/*
title Login Process
loop req != 1
Client-> + Server: PACKET_LOGIN_REQ
Server->DB : PACKET_DB_LOGIN_REQ
DB->Server : PACKET_DB_LOGIN_ACK
Server-> - Client : PACKET_LOGIN_ACK
end
Client->Client : LoginProcess
*/