#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "TLobbyServer.h"
#include "TAccepter.h"

void main()
{
	TLobbyServer server;
	server.InitServer(10000);
	TAccepter accept(&server);
	accept.Detach();
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