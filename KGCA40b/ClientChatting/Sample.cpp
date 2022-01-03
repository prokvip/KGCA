#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <conio.h>
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
	if (iRet == SOCKET_ERROR)
	{
		return;
	}
	std::cout << "접속성공!" << std::endl;

	u_long on = 1;
	ioctlsocket(sock, FIONBIO, &on);

	char szBuffer[256] = { 0, };
	int iEnd = 0;
	while (1)
	{				
		if (_kbhit() == 1)
		{
			int iValue = _getche();	
			if (iValue == '\r' && strlen(szBuffer) == 0)
			{
				break;
			}
			if (iValue == '\r')
			{
				int iSendByte = send(sock,
					szBuffer,
					iEnd,
					0);

				if (iSendByte == SOCKET_ERROR)
				{
					if (WSAGetLastError() != WSAEWOULDBLOCK)
					{
						std::cout << "비정상 서버종료됨.." << std::endl;
					}
				}
				iEnd = 0;
				ZeroMemory(szBuffer, sizeof(char) * 256);
			}
			else
			{
				szBuffer[iEnd++] = iValue;
			}
		}
		else
		{
			char szRecvBuffer[256] = { 0, };
			int iRecvByte = recv(sock, szRecvBuffer, 256, 0);
			
			if (iRecvByte == 0)
			{
				std::cout << "서버종료됨.." << std::endl;
				break;
			}
			if (iRecvByte == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					std::cout << "비정상 서버종료됨.." << std::endl;
				}
			}
			else
			{
				std::cout << "\n" <<szRecvBuffer;
				ZeroMemory(szRecvBuffer, sizeof(char) * 256);
			}
		}
	}
	closesocket(sock);
	WSACleanup();
}