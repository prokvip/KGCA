#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#pragma comment	(lib, "ws2_32.lib")
void main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	SOCKET sock = socket(AF_INET,
		SOCK_STREAM, 0); //SOCK_STREAM, SOCK_DGRAM
			 //IPPROTO_TCP,IPPROTO_UDP
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);	
	sa.sin_addr.s_addr = inet_addr("192.168.0.12");
	int iRet = connect(
		sock,
		(sockaddr*)&sa,
		sizeof(sa));
	char szBuffer[] = "æ»≥Á«œººø©.";
	int iSendByte = send(sock,
		szBuffer,
		sizeof(szBuffer),
		0);
	char szRecvBuffer[256] = { 0, };
	int iRecvByte = recv(sock, szRecvBuffer, 256, 0);
	std::cout << szRecvBuffer;
	WSACleanup();
}