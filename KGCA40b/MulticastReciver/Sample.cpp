#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
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

	SOCKET sockRecv= socket(AF_INET, SOCK_DGRAM, 0);
	char opt = 1;
	int optlevel = IPPROTO_IP;
	int iption = IP_MULTICAST_LOOP;
	int optlen = sizeof(opt);
	int iRet = setsockopt(sockRecv, IPPROTO_IP, IP_MULTICAST_LOOP, &opt, sizeof(opt));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}

	int iRet = -1;
	SOCKADDR_IN address;
	ZeroMemory(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(9000);
	address.sin_addr.s_addr = INADDR_ANY;// inet_addr("192.168.0.12");
	iRet = bind(sockRecv, (SOCKADDR*)&address, sizeof(address));

	ip_mreq mreq;
	mreq.imr_interface.s_addr = inet_addr("235.7.8.9");
	mreq.imr_multiaddr.s_addr = inet_addr("192.168.0.12");
	setsockopt(sockRecv, IPPROTO_IP, IP_ADD_MEMBERSHIP, 
		(char*)&mreq, sizeof(mreq) );

	// include ��� ����.	
	/*ip_mreq_source mreqsource;
	mreqsource.imr_interface = mreq.imr_interface;
	mreqsource.imr_multiaddr = mreq.imr_multiaddr;
	mreqsource.imr_sourceaddr.s_addr = inet_addr("192.168.0.40");
	setsockopt(sockRecv, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP,
		(char*)&mreq, sizeof(mreq));
	mreqsource.imr_sourceaddr.s_addr = inet_addr("192.168.0.41");
	setsockopt(sockRecv, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP,
		(char*)&mreq, sizeof(mreq));
	mreqsource.imr_sourceaddr.s_addr = inet_addr("192.168.0.42");
	setsockopt(sockRecv, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP,
		(char*)&mreq, sizeof(mreq));*/

	// exclude ��� ����
	ip_mreq_source mreqsource;
	mreqsource.imr_interface = mreq.imr_interface;
	mreqsource.imr_multiaddr = mreq.imr_multiaddr;
	mreqsource.imr_sourceaddr.s_addr = inet_addr("192.168.0.40");
	setsockopt(sockRecv, IPPROTO_IP, IP_BLOCK_SOURCE,
		(char*)&mreq, sizeof(mreq));

	
	SOCKADDR_IN multicastAddress;
	ZeroMemory(&multicastAddress, sizeof(multicastAddress));
	multicastAddress.sin_family = AF_INET;
	multicastAddress.sin_port = htons(9000);
	multicastAddress.sin_addr.s_addr = inet_addr("235.7.8.9");


	SOCKADDR_IN recvAddr;
	char buf[256] = { 0, };
	INT addlen = sizeof(recvAddr);
	while (1)
	{
		iRet = recvfrom(sockRecv, buf, 256, 0, (SOCKADDR*)&recvAddr, &addlen);
		if (iRet == SOCKET_ERROR)
		{
			break;
		}
		buf[iRet] = 0;
		printf("\n[%s:%d]:%s", inet_ntoa(recvAddr.sin_addr), ntohs(recvAddr.sin_port), buf);
		iRet = sendto(sockRecv, buf, iRet, 0,
			(SOCKADDR*)&multicastAddress, sizeof(multicastAddress));
		if (iRet == SOCKET_ERROR)
		{
			break;
		}
	}


	setsockopt(sockRecv, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&mreq, sizeof(mreq));
	WSACleanup();
}