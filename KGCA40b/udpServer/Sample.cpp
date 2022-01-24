#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

void main()
{
	WSADATA wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0)
	{
		return;
	}

	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	SOCKADDR_IN address;
	ZeroMemory(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(9000);
	address.sin_addr.s_addr = inet_addr("192.168.0.12");
	int iRet = bind(sock, (SOCKADDR*)&address, sizeof(address));

	SOCKADDR_IN recvAddr;
	char buf[256] = { 0, };
	INT addlen = sizeof(recvAddr);
	while (1)
	{
		iRet = recvfrom(sock, buf, 256, 0, (SOCKADDR*)&recvAddr, &addlen);
		if (iRet == SOCKET_ERROR)
		{
			break;
		}
		buf[iRet] = 0;
		printf("\n[%s:%d]:%s", inet_ntoa(recvAddr.sin_addr), ntohs(recvAddr.sin_port), buf);
		iRet = sendto(sock, buf, iRet, 0, (SOCKADDR*)&recvAddr, sizeof(recvAddr));
		if (iRet == SOCKET_ERROR)
		{
			break;
		}
	}

	WSACleanup();
}