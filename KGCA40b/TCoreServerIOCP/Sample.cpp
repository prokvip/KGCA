#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "TServer.h"
#include "TThreadRecv.h"
#include "TThreadSend.h"

void main()
{
	TServer server;
	server.InitServer(10000);
	
	TThreadRecv tRecv(&server);
	TThreadSend tSend(&server);
	tRecv.Detach();
	tSend.Detach();
	
	server.Run();
	server.Release();

	//tRecv.Join();
	//tSend.Join();
}