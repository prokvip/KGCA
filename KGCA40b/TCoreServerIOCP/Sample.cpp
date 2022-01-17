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