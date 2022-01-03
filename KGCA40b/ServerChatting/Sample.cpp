#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <list>
#pragma comment	(lib, "ws2_32.lib")
void main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	SOCKET ListenSock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	int iRet = bind(ListenSock, (sockaddr*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)  return;
	iRet = listen(ListenSock, SOMAXCONN);
	if (iRet == SOCKET_ERROR)  return;

	SOCKADDR_IN clientAddr;
	int iLen = sizeof(clientAddr);

	std::cout
		<< "서버 가동중......." << std::endl;

	u_long on = 1;
	ioctlsocket(ListenSock, FIONBIO, &on);

	std::list<SOCKET> userlist;
	//while (userlist.size() > 2)
	//{
	//	SOCKET clientSock = accept(ListenSock,
	//		(sockaddr*)&clientAddr, &iLen);
	//	if (clientSock == SOCKET_ERROR)
	//	{
	//		int iError = WSAGetLastError();
	//		if (iError != WSAEWOULDBLOCK)
	//		{
	//			std::cout << "ErrorCode=" << iError << std::endl;
	//			break;
	//		}
	//	}
	//	else
	//	{
	//		userlist.push_back(clientSock);
	//		std::cout
	//			<< "ip =" << inet_ntoa(clientAddr.sin_addr)
	//			<< "port =" << ntohs(clientAddr.sin_port)
	//			<< "  " << std::endl;
	//		u_long on = 1;
	//		ioctlsocket(clientSock, FIONBIO, &on);
	//		std::cout << userlist.size() << " 명 접속중.." << std::endl;
	//	}
	//}

	while (1)
	{
		SOCKET clientSock = accept(ListenSock,
			(sockaddr*)&clientAddr, &iLen);
		if (clientSock == SOCKET_ERROR)
		{
			int iError = WSAGetLastError();
			if (iError != WSAEWOULDBLOCK)
			{
				std::cout << "ErrorCode=" << iError << std::endl;
				break;
			}
		}
		else
		{
			userlist.push_back(clientSock);
			std::cout
				<< "ip =" << inet_ntoa(clientAddr.sin_addr)
				<< "port =" << ntohs(clientAddr.sin_port)
				<< "  " << std::endl;
			u_long on = 1;
			ioctlsocket(clientSock, FIONBIO, &on);
			std::cout << userlist.size() << " 명 접속중.." << std::endl;
		}

		if(userlist.size() > 0)
		{
			std::list<SOCKET>::iterator iter;
			for (iter = userlist.begin(); iter != userlist.end(); )
			{
				SOCKET sock = *iter;
				char szRecvBuffer[256] = { 0, };
				int iRecvByte = recv(sock, szRecvBuffer, 256, 0);
				if (iRecvByte == 0)
				{
					closesocket(sock);
					iter = userlist.erase(iter);
					continue;
				}
				if (iRecvByte == SOCKET_ERROR)
				{
					int iError = WSAGetLastError();
					//std::cout << "ErrorCode=" << iError << std::endl;
					if (iError != WSAEWOULDBLOCK)
					{
						iter = userlist.erase(iter);
					}
					else
					{
						iter++;
					}
				}
				else
				{
					std::list<SOCKET>::iterator iterSend;
					for (iterSend = userlist.begin(); 
						iterSend != userlist.end();	)
					{
						SOCKET sock = *iterSend;
						std::cout << szRecvBuffer << "받음" << std::endl;
						int iSendByte = send(sock, szRecvBuffer, iRecvByte, 0);
						std::cout << sock <<":" <<iSendByte << "보냄." << std::endl;
						if (iSendByte == SOCKET_ERROR)
						{
							int iError = WSAGetLastError();
							if (iError != WSAEWOULDBLOCK)
							{
								closesocket(sock);
								iterSend = userlist.erase(iterSend);
							}
						}
						else
						{
							iterSend++;
						}
					}				
					if (iter != userlist.end())
					{
						iter++;
					}
				}				
			}			
		}		
	}
	closesocket(ListenSock);
	WSACleanup();
}