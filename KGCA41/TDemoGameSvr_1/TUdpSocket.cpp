#include "StdAfx.h"
#include "TBasisSvr.h"
#include "TUdpSocket.h"

//IP_ADD_MEMBERSHIP		- 멀티캐스트 그룹에 가입
//IP_DROP_MEMBERSHIP	- 멀티캐스트 그룹에서 탈퇴
//IP_MULTICAST_LOOP		- 멀티캐스트 패킷의 loopback 허용 여부 지정
//IP_MULTICAST_TTL		- 멀티캐스트 패킷의 TTL 값 지정
//IP_MULTICAST_IF		- 멀티캐스트 패킷 전송용 인터페이스 지정

// 포트 번호( 0 ~ 65535 )
// 0		~ 1023	: 지정(잘알려진) 포트 번호
// 1024		~ 49151 : 등록된 포트 번호
// 49152	~ 65535 : 동적 포트 번호( 자유롭게 사용 가능 )
bool TUdpSocket::Init()
{
	int retval;
	m_Socket = socket( AF_INET, SOCK_DGRAM, 0);
	int ttl = 64;
	retval = setsockopt( m_Socket, IPPROTO_IP, IP_MULTICAST_TTL,(char*)&ttl, sizeof(ttl));

	ZeroMemory( &m_RecvAddr, sizeof(m_RecvAddr));
	m_RecvAddr.sin_family = AF_INET;
	m_RecvAddr.sin_port = htons(9001);
	m_RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = ::bind(m_Socket, (SOCKADDR*)&m_RecvAddr,	sizeof(m_RecvAddr));
	if( retval == SOCKET_ERROR )
	{
		T_ERR;
	}

	// 멀티 캐스트 그룹 생성	
	// 224.0.0.0 ~ 239.255.255.255(D클래스: 선두 4비트는 1110이 된다.)
	// 실제 멀티 캐스팅 응용을 위햇 할당된 주소
	// 224.0.1.0 ~ 238.255.255.255 : 실제 멀티캐스트 어플리케이션 용도
	// 232.0.0.0 ~ 232.255.255.255 : SSM(Source Specific Multicast)용도
	// 233.0.0.0 ~ 233.255.255.255 : 하나의 AS 전체에 할당되는 주소
	// 239.0.0.0 ~ 239.255.255.255 : 사설망에서 사용(Private, Administrative Scoping)
	ZeroMemory( &m_Remoteaddr, sizeof(m_Remoteaddr));
	m_Remoteaddr.sin_family = AF_INET;
	m_Remoteaddr.sin_port = htons(10000);
	//m_Remoteaddr.sin_addr.s_addr = inet_addr("235.7.8.9");
	InetPton(AF_INET, _T("235.7.8.9"), &(m_Remoteaddr.sin_addr));
	// 가입	
	// 그룹에 가입할 호스트 주소.
	m_MemberReq.imr_interface.s_addr = htonl(INADDR_ANY);
	// 멀티캐스트 그룹의 주소
	//m_MemberReq.imr_multiaddr.s_addr = inet_addr("235.7.8.9");
	InetPton(AF_INET, _T("235.7.8.9"), &(m_MemberReq.imr_multiaddr));
	retval= setsockopt( m_Socket, IPPROTO_IP, IP_ADD_MEMBERSHIP,(char*)&m_MemberReq, sizeof( m_MemberReq ));
	if( retval == SOCKET_ERROR )
	{
		T_ERR;
	}

	// udp 가동
	//CreateThread();
	return true;
}
bool TUdpSocket::Run()
{
	int retval;
	int addrlen;
	SOCKADDR_IN PeerAddr;
	char buf[2048];
	while(I_Server.GetAcceptor().m_bStarted)
	{
		addrlen = sizeof( PeerAddr );
		retval = recvfrom(	m_Socket, buf, 2048, 0,(SOCKADDR*)&PeerAddr, &addrlen);
		if( retval == SOCKET_ERROR )
		{
			T_ERR;
			break;
		}
		buf[retval] = 0;
		I_DebugStr.DisplayText("Message:%s", buf );

		// 송신자는 멀티캐스트 그룹에 가입할 필요가 없으나
		// 이곳에서는 다시 받기 위해서 가입하였다.
		retval = sendto( m_Socket, buf, (int)strlen(buf), 0,(SOCKADDR*)&m_Remoteaddr, sizeof(m_Remoteaddr));				
		if( retval == SOCKET_ERROR )
		{
			T_ERR;
		}
	}
	return true;
}
bool TUdpSocket::Release()
{
	// 멀티캐스트 그룹 탈퇴
	int retval = setsockopt(m_Socket, IPPROTO_IP, IP_DROP_MEMBERSHIP,(char *)&m_MemberReq, sizeof(m_MemberReq));
	if(retval == SOCKET_ERROR) {
		T_ERR;
	}
	closesocket( m_Socket);
	return true;
}
TUdpSocket::TUdpSocket(void)
{
}

TUdpSocket::~TUdpSocket(void)
{
}
