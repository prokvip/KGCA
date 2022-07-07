#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
void Error(const char* msg)
{
	LPVOID lpMsgBuffer;
	int iError = WSAGetLastError();
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		iError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&lpMsgBuffer,
		0, NULL);
	printf("%s", lpMsgBuffer);
	LocalFree(lpMsgBuffer);
	//exit(-1);
}
void main()
{
	WSADATA wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0)
	{
		return;
	}

	SOCKET sockRecv= socket(AF_INET, SOCK_DGRAM, 0);
	SOCKET sockSend = socket(AF_INET, SOCK_DGRAM, 0);
	int val = 1;
	char* opt = (char*)&val;
	int optlevel = IPPROTO_IP;
	int iption = IP_MULTICAST_LOOP;
	int optlen = sizeof(opt);
	int iRet = setsockopt(sockRecv, IPPROTO_IP, 
		IP_MULTICAST_LOOP, opt, sizeof(opt));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}

	SOCKADDR_IN address;
	ZeroMemory(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(9000);
	address.sin_addr.s_addr = INADDR_ANY;// inet_addr("192.168.0.12");
	iRet = bind(sockRecv, (SOCKADDR*)&address, sizeof(address));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}
	ip_mreq mreq;
	mreq.imr_interface.s_addr = inet_addr("192.168.0.12");
	mreq.imr_multiaddr.s_addr = inet_addr("235.7.8.9");
	iRet = setsockopt(sockRecv, IPPROTO_IP, IP_ADD_MEMBERSHIP, 
		(char*)&mreq, sizeof(mreq) );
	if (iRet == SOCKET_ERROR)
	{
		Error("error");
		return;
	}
	// include 대상 포함.	
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

	// exclude 대상 삭제
	ip_mreq_source mreqsource;
	mreqsource.imr_interface = mreq.imr_interface;
	mreqsource.imr_multiaddr = mreq.imr_multiaddr;
	mreqsource.imr_sourceaddr.s_addr = inet_addr("192.168.0.40");
	iRet = setsockopt(sockRecv, IPPROTO_IP, IP_BLOCK_SOURCE,
		(char*)&mreqsource, sizeof(mreqsource));
	if (iRet == SOCKET_ERROR)
	{
		Error("error");
		return;
	}
	
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
		iRet = sendto(sockSend, buf, iRet, 0,
			(SOCKADDR*)&multicastAddress, sizeof(multicastAddress));
		if (iRet == SOCKET_ERROR)
		{
			break;
		}
		printf("\n[%s:%d]:%s", inet_ntoa(recvAddr.sin_addr), ntohs(recvAddr.sin_port), buf);
	}


	setsockopt(sockRecv, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&mreq, sizeof(mreq));
	WSACleanup();
}