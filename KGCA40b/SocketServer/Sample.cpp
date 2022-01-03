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
	SOCKET ListenSock = socket(AF_INET,SOCK_STREAM, 0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	int iRet = bind(ListenSock,(sockaddr*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)  return;
	iRet = listen(ListenSock, SOMAXCONN);
	if (iRet == SOCKET_ERROR)  return;

	SOCKADDR_IN clientAddr;
	int iLen = sizeof(clientAddr);

	std::cout
		<< "서버 가동중......." << std::endl;

	while (1)
	{		
		SOCKET clientSock = accept(ListenSock,
			(sockaddr*)&clientAddr, &iLen);
		if (clientSock == SOCKET_ERROR)
		{
			int iError = WSAGetLastError();
			std::cout << "ErrorCode=" << iError << std::endl;
			if (iError != WSAEWOULDBLOCK)
			{
				break;
			}
			continue;
		}
		std::cout 
			<< "ip =" <<  inet_ntoa(clientAddr.sin_addr) 
			<< "port ="<< ntohs(clientAddr.sin_port) 
			<< "  " << std::endl;
		
		u_long on = 1;
		ioctlsocket(clientSock, FIONBIO, &on);

		while (1)
		{
			char szRecvBuffer[256] = { 0, };
			int iRecvByte = recv(clientSock, szRecvBuffer, 256, 0);
			if (iRecvByte == 0 )
			{
				std::cout
					<< "ip =" << inet_ntoa(clientAddr.sin_addr)
					<< "port =" << ntohs(clientAddr.sin_port)
					<< "==>logout " << std::endl;
				break;
			}
			if (iRecvByte == SOCKET_ERROR)
			{
				int iError = WSAGetLastError();
				//std::cout << "ErrorCode=" << iError << std::endl;
				if (iError != WSAEWOULDBLOCK)
				{
					break;
				}				
			}
			else
			{
				std::cout << szRecvBuffer << "받음" << std::endl;
				int iSendByte = send(clientSock, szRecvBuffer, iRecvByte, 0);
				std::cout << iSendByte << "보냄." << std::endl;
			}			
		}
		closesocket(clientSock);
	}
	/*char szBuffer[] = "안녕하세여.";
	int iSendByte = send(sock,
		szBuffer,
		sizeof(szBuffer),
		0);
	char szRecvBuffer[256] = { 0, };
	int iRecvByte = recv(sock, szRecvBuffer, 256, 0);
	std::cout << szRecvBuffer;*/
	WSACleanup();
}