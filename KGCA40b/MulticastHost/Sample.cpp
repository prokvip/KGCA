#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <process.h>
#pragma comment(lib, "ws2_32.lib")

bool gMode =  MCAST_EXCLUDE;//MCAST_INCLUDE
struct ip_msfilter* g_filter = NULL;

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

struct addrinfo* ResolveAddress(char* addr, char* port, int af, int type, int proto)
{
	struct addrinfo hints, * res = NULL;
	int             rc;

	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = ((addr) ? 0 : AI_PASSIVE);
	hints.ai_family = af;
	hints.ai_socktype = type;
	hints.ai_protocol = proto;

	rc = getaddrinfo(addr, port, &hints, &res);
	if (rc != 0)
	{
		printf("Invalid address %s, getaddrinfo failed: %d\n", addr, rc);
		return NULL;
	}
	return res;
}
unsigned WINAPI SendThread(LPVOID arg)
{
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	char SendBuffer[256] = { 0, };

	SOCKADDR_IN multicastAddress;
	ZeroMemory(&multicastAddress, sizeof(multicastAddress));
	multicastAddress.sin_family = AF_INET;
	multicastAddress.sin_port = htons(9000);
	multicastAddress.sin_addr.s_addr = inet_addr("235.7.8.9");
	{
		while (1)
		{
			fgets(SendBuffer, 256, stdin);
			int iRet = sendto(sock, SendBuffer, strlen(SendBuffer), 0,
				(SOCKADDR*)&multicastAddress, sizeof(multicastAddress));
			if (iRet == SOCKET_ERROR)
			{
				break;
			}
		}
	}
	return 0;
}
void GetMulticastState(SOCKET s, struct addrinfo* group, struct addrinfo* iface)
{
	struct ip_msfilter* filter = NULL;
	char                buf[15000];
	int                 buflen = 15000, rc, i;

	filter = (struct ip_msfilter*)buf;

	filter->imsf_multiaddr = ((SOCKADDR_IN*)group->ai_addr)->sin_addr;
	filter->imsf_interface = ((SOCKADDR_IN*)iface->ai_addr)->sin_addr;

	rc = WSAIoctl(s, SIO_GET_MULTICAST_FILTER, buf, buflen, buf, buflen, (LPDWORD)&buflen, NULL, NULL);
	if (rc == SOCKET_ERROR)
	{
		fprintf(stderr, "GetMulticastState: WSAIoctl() failed with error code %d\n", WSAGetLastError());
		return;
	}

	printf("imsf_multiaddr = %s\n", inet_ntoa(filter->imsf_multiaddr));
	printf("imsf_interface = %s\n", inet_ntoa(filter->imsf_interface));
	printf("imsf_fmode     = %s\n", (filter->imsf_fmode == MCAST_INCLUDE ? "MCAST_INCLUDE" : "MCAST_EXCLUDE"));
	printf("imsf_numsrc    = %d\n", filter->imsf_numsrc);
	for (i = 0; i < (int)filter->imsf_numsrc; i++)
	{
		printf("imsf_slist[%d]  = %s\n", i, inet_ntoa(filter->imsf_slist[i]));
	}

	g_filter = filter;
	return;
}
struct addrinfo* ResolveAddress(const char* addr, const char* port, int af, int type, int proto)
{
	struct addrinfo hints, * res = NULL;
	int             rc;

	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = ((addr) ? 0 : AI_PASSIVE);
	hints.ai_family = af;
	hints.ai_socktype = type;
	hints.ai_protocol = proto;

	rc = getaddrinfo(addr, port, &hints, &res);
	if (rc != 0)
	{
		printf("Invalid address %s, getaddrinfo failed: %d\n", addr, rc);
		return NULL;
	}
	return res;
}

void main(int argc, char** argv)
{
	WSADATA wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0)
	{
		return;
	}
	SOCKET sockRecv = socket(AF_INET, SOCK_DGRAM, 0);	
	// 루프백 기능 취소(기본 = true)
	char opt = 1;	
	int iRet = setsockopt(sockRecv, IPPROTO_IP,	IP_MULTICAST_LOOP, &opt, sizeof(opt));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}

	SOCKADDR_IN address;
	ZeroMemory(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(9000);
	address.sin_addr.s_addr = INADDR_ANY;
	iRet = bind(sockRecv, (SOCKADDR*)&address, sizeof(address));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}

	ip_mreq mreq;
	ip_mreq_source mreqsource;
	if (gMode == MCAST_INCLUDE)
	{		
		mreqsource.imr_interface.s_addr = inet_addr("192.168.0.12");
		mreqsource.imr_multiaddr.s_addr = inet_addr("235.7.8.9");
		mreqsource.imr_sourceaddr.s_addr = inet_addr("192.168.0.40");
		setsockopt(sockRecv, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP,	(char*)&mreqsource, sizeof(mreqsource));
		mreqsource.imr_sourceaddr.s_addr = inet_addr("192.168.0.41");
		setsockopt(sockRecv, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP,	(char*)&mreqsource, sizeof(mreqsource));
		mreqsource.imr_sourceaddr.s_addr = inet_addr("192.168.0.42");
		setsockopt(sockRecv, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP,	(char*)&mreqsource, sizeof(mreqsource));

		/*mreqsource.imr_sourceaddr.s_addr = inet_addr("192.168.0.42");
		setsockopt(sockRecv, IPPROTO_IP, IP_DROP_SOURCE_MEMBERSHIP, (char*)&mreqsource, sizeof(mreqsource));*/
	}
	if (gMode == MCAST_EXCLUDE)
	{
		mreq.imr_interface.s_addr = inet_addr("192.168.0.12");
		mreq.imr_multiaddr.s_addr = inet_addr("235.7.8.9");
		iRet = setsockopt(sockRecv, IPPROTO_IP, IP_ADD_MEMBERSHIP,	(char*)&mreq, sizeof(mreq));
		if (iRet == SOCKET_ERROR)
		{
			Error("error");
			return;
		}
		ip_mreq_source mreqsource;
		mreqsource.imr_interface = mreq.imr_interface;
		mreqsource.imr_multiaddr = mreq.imr_multiaddr;
		mreqsource.imr_sourceaddr.s_addr = inet_addr("192.168.0.115");
		iRet = setsockopt(sockRecv, IPPROTO_IP, IP_BLOCK_SOURCE, (char*)&mreqsource, sizeof(mreqsource));
		if (iRet == SOCKET_ERROR)
		{
			Error("error");
			return;
		}

		/*mreqsource.imr_sourceaddr.s_addr = inet_addr("192.168.0.115");
		iRet = setsockopt(sockRecv, IPPROTO_IP, IP_UNBLOCK_SOURCE, (char*)&mreqsource, sizeof(mreqsource));
		if (iRet == SOCKET_ERROR)
		{
			Error("error");
			return;
		}*/
	}
	
	addrinfo* resmulti = ResolveAddress("235.7.8.9", "9000", AF_UNSPEC, SOCK_DGRAM, IPPROTO_UDP);	
	addrinfo* resif = ResolveAddress("192.168.0.12", "9000", AF_UNSPEC, SOCK_DGRAM, IPPROTO_UDP);
	GetMulticastState(sockRecv, resmulti, resif);

	unsigned int id;
	unsigned long hSendThread = _beginthreadex(NULL, 0, SendThread, 0, 0, &id);


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
		Sleep(100);
		
		// IP_ADD_MEMBERSHIP 으로 등록되어 있어야만 IP_BLOCK_SOURCE 할수 있다.
		for (int i = 0; i < (int)g_filter->imsf_numsrc; i++)
		{
			printf("imsf_slist[%d]  = %s\n", i, inet_ntoa(g_filter->imsf_slist[i]));
			if (recvAddr.sin_addr.s_addr == g_filter->imsf_slist[i].s_addr)
			{
				ip_mreq_source mreqsource;
				mreqsource.imr_interface = mreq.imr_interface;
				mreqsource.imr_multiaddr = mreq.imr_multiaddr;
				mreqsource.imr_sourceaddr = g_filter->imsf_slist[i];
				iRet = setsockopt(sockRecv, IPPROTO_IP, IP_BLOCK_SOURCE, (char*)&mreqsource, sizeof(mreqsource));
				if (iRet == SOCKET_ERROR)
				{
					Error("error");					
					return;
				}
			}
		}

		printf("\n[%s:%d]:%s", inet_ntoa(recvAddr.sin_addr), ntohs(recvAddr.sin_port), buf);
	}

	setsockopt(sockRecv, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&mreq, sizeof(mreq));
	WSACleanup();
}