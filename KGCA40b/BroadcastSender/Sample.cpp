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
	char flag = true;
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag)) < 0)
	{
		closesocket(sock);
	}
	int iRet = -1;
	SOCKADDR_IN address;
	ZeroMemory(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(9000);
	address.sin_addr.s_addr = INADDR_BROADCAST;
	//iRet = bind(sock, (SOCKADDR*)&address, sizeof(address));

	SOCKADDR_IN recvAddr;
	char SendBuf[256] = { 0, };
	char RecvBuf[256] = { 0, };
	INT addlen = sizeof(recvAddr);
	int iCount = 0;

	while (1)
	{
		sprintf(SendBuf, "%s:%d", "ÇÐ¿ø", iCount++);
		int iRet = sendto(sock, SendBuf, strlen(SendBuf), 0,
			(SOCKADDR*)&address, sizeof(address));
		if (iRet == SOCKET_ERROR)
		{
			break;
		}

		iRet = recvfrom(sock, RecvBuf, 256, 0, (SOCKADDR*)&recvAddr, &addlen);
		if (iRet == SOCKET_ERROR)
		{
			break;
		}
		RecvBuf[iRet] = 0;
		printf("\n[%s:%d]:%s", inet_ntoa(recvAddr.sin_addr),
			ntohs(recvAddr.sin_port), RecvBuf);
		Sleep(1000);
	}

	WSACleanup();
}