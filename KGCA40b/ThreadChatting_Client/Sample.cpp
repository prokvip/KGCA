#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <conio.h>
#include "TPacket.h"
#pragma comment	(lib, "ws2_32.lib")
int SendMsg(SOCKET sock, char* msg, WORD type)
{
	// 1�� ��Ŷ ����
	UPACKET packet;
	ZeroMemory(&packet, sizeof(packet));
	packet.ph.len = strlen(msg) + PACKET_HEADER_SIZE;
	packet.ph.type = type;
	memcpy(packet.msg, msg, strlen(msg));	
	// 2�� ��Ŷ ���� : �ü�� sendbuffer(short����Ʈ), recvbuffer
	char* pMsg = (char*)&packet;
	int iSendSize = 0;
	do {
		int iSendByte = send(sock, &pMsg[iSendSize],
			packet.ph.len - iSendSize, 0);
		if (iSendByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
		}
		iSendSize += iSendByte;
	} while (iSendSize < packet.ph.len);
	return iSendSize;
}
int SendPacket(SOCKET sock, char* msg, WORD type)
{
	// 1�� ��Ŷ ����
	TPacket tPacket(type);
	tPacket << 999 << "ȫ�浿" << (short)50 << msg;
	TPacket tPacketTest(tPacket);
	TChatMsg recvdata;
	ZeroMemory(&recvdata, sizeof(recvdata));
	tPacketTest >> recvdata.index >> recvdata.name
		>> recvdata.damage >> recvdata.message;
	char* pData = (char*)&tPacket.m_uPacket;
	int iSendSize = 0;
	do {
		int iSendByte = send(sock, &pData[iSendSize],
			tPacket.m_uPacket.ph.len - iSendSize, 0);
		if (iSendByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
		}
		iSendSize += iSendByte;
	} while (iSendSize < tPacket.m_uPacket.ph.len);
	return iSendSize;
}
int RecvPacketHeader(SOCKET sock, UPACKET& recvPacket)
{
	char szRecvBuffer[256] = { 0, };
	//��Ŷ��� �ޱ�
	ZeroMemory(&recvPacket, sizeof(recvPacket));
	bool bRun = true;
	int iRecvSize = 0;
	do {
		int iRecvByte = recv(sock, szRecvBuffer,
			PACKET_HEADER_SIZE, 0);
		iRecvSize += iRecvByte;
		if (iRecvByte == 0)
		{
			closesocket(sock);
			std::cout << " ���������." << std::endl;
			return -1;
		}
		if (iRecvByte == SOCKET_ERROR)
		{
			int iError = WSAGetLastError();
			if (iError != WSAEWOULDBLOCK)
			{
				std::cout << " ������ ���������." << std::endl;
				return -1;
			}
			else
			{
				return 0;
			}
		}
	} while (iRecvSize < PACKET_HEADER_SIZE);
	memcpy(&recvPacket.ph, szRecvBuffer, PACKET_HEADER_SIZE);
	return 1;
}
int RecvPacketData(SOCKET sock, UPACKET& recvPacket)
{
	// ������ �ޱ�
	int iRecvSize = 0;
	do {
		int iRecvByte = recv(sock, recvPacket.msg,
			recvPacket.ph.len - PACKET_HEADER_SIZE - iRecvSize, 0);
		iRecvSize += iRecvByte;
		if (iRecvByte == 0)
		{
			closesocket(sock);
			std::cout << " ���������." << std::endl;
			return -1;
		}
		if (iRecvByte == SOCKET_ERROR)
		{
			int iError = WSAGetLastError();
			if (iError != WSAEWOULDBLOCK)
			{
				std::cout << " ������ ���������." << std::endl;
				return -1;
			}
			else
			{
				return 0;
			}
		}
	} while (iRecvSize < recvPacket.ph.len - PACKET_HEADER_SIZE);
	return 1;
}

DWORD WINAPI SendThread(LPVOID lpThreadParameter)
{
	SOCKET sock = (SOCKET)lpThreadParameter;
	char szBuffer[256] = { 0, };
	while (1)
	{
		ZeroMemory(szBuffer, sizeof(char) * 256);
		fgets(szBuffer, 256, stdin);
		if (strlen(szBuffer) == 0)
		{
			std::cout << "���� �����.." << std::endl;
			break;
		}
		
		// ��� 1
		//int iSendByte = SendMsg(sock, szBuffer, PACKET_CHAT_MSG);
		// ��� 2
		int iSendByte = SendPacket(sock, szBuffer, PACKET_CHAT_MSG);
		if (iSendByte < 0)
		{
			std::cout << "������ �����.." << std::endl;
			break;
		}				
	}
	return 0;
}
DWORD WINAPI RecvThread(LPVOID param)
{
	SOCKET sock = (SOCKET)param;
	while (1)
	{
		UPACKET packet;
		int iRet = RecvPacketHeader(sock, packet);
		if (iRet < 0) continue;
		if (iRet == 1)
		{
			int iRet = RecvPacketData(sock, packet);
			if (iRet < 0) break;
			if (iRet == 0) continue;
			// �޼��� ó��
			TPacket data;
			data.m_uPacket = packet;
			TChatMsg recvdata;
			ZeroMemory(&recvdata, sizeof(recvdata));
			data >> recvdata.index >> recvdata.name
				>> recvdata.damage >> recvdata.message;
			std::cout << "\n" <<
				"[" << recvdata.name << "]"
				<< recvdata.message;
		}
	}
	return 0;
}
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
	std::cout << "���Ӽ���!" << std::endl;

	u_long on = 1;
	ioctlsocket(sock, FIONBIO, &on);

	// ������ ����
	// 1)window api
	DWORD ThreadId;
	HANDLE hThread = ::CreateThread(
		0,
		0,
		SendThread,// ��ȯ
		(LPVOID)sock,
		0,
		&ThreadId
	);
	DWORD ThreadIdRecv;
	HANDLE hThreadRecv = ::CreateThread(
		0,
		0,
		RecvThread,// ��ȯ
		(LPVOID)sock,
		0,
		&ThreadIdRecv
	);
	// 2)c++11

	// ���ν����� �۾�
	while (1)
	{		
		Sleep(1);
	}
	std::cout << "��������" << std::endl;
	
	CloseHandle(hThread);
	closesocket(sock);
	WSACleanup();
	//_getch();
}