#include "StdAfx.h"
#include "TBasisSvr.h"
#include "TUdpSocket.h"

//IP_ADD_MEMBERSHIP		- ��Ƽĳ��Ʈ �׷쿡 ����
//IP_DROP_MEMBERSHIP	- ��Ƽĳ��Ʈ �׷쿡�� Ż��
//IP_MULTICAST_LOOP		- ��Ƽĳ��Ʈ ��Ŷ�� loopback ��� ���� ����
//IP_MULTICAST_TTL		- ��Ƽĳ��Ʈ ��Ŷ�� TTL �� ����
//IP_MULTICAST_IF		- ��Ƽĳ��Ʈ ��Ŷ ���ۿ� �������̽� ����

// ��Ʈ ��ȣ( 0 ~ 65535 )
// 0		~ 1023	: ����(�߾˷���) ��Ʈ ��ȣ
// 1024		~ 49151 : ��ϵ� ��Ʈ ��ȣ
// 49152	~ 65535 : ���� ��Ʈ ��ȣ( �����Ӱ� ��� ���� )
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

	// ��Ƽ ĳ��Ʈ �׷� ����	
	// 224.0.0.0 ~ 239.255.255.255(DŬ����: ���� 4��Ʈ�� 1110�� �ȴ�.)
	// ���� ��Ƽ ĳ���� ������ ���� �Ҵ�� �ּ�
	// 224.0.1.0 ~ 238.255.255.255 : ���� ��Ƽĳ��Ʈ ���ø����̼� �뵵
	// 232.0.0.0 ~ 232.255.255.255 : SSM(Source Specific Multicast)�뵵
	// 233.0.0.0 ~ 233.255.255.255 : �ϳ��� AS ��ü�� �Ҵ�Ǵ� �ּ�
	// 239.0.0.0 ~ 239.255.255.255 : �缳������ ���(Private, Administrative Scoping)
	ZeroMemory( &m_Remoteaddr, sizeof(m_Remoteaddr));
	m_Remoteaddr.sin_family = AF_INET;
	m_Remoteaddr.sin_port = htons(10000);
	//m_Remoteaddr.sin_addr.s_addr = inet_addr("235.7.8.9");
	InetPton(AF_INET, _T("235.7.8.9"), &(m_Remoteaddr.sin_addr));
	// ����	
	// �׷쿡 ������ ȣ��Ʈ �ּ�.
	m_MemberReq.imr_interface.s_addr = htonl(INADDR_ANY);
	// ��Ƽĳ��Ʈ �׷��� �ּ�
	//m_MemberReq.imr_multiaddr.s_addr = inet_addr("235.7.8.9");
	InetPton(AF_INET, _T("235.7.8.9"), &(m_MemberReq.imr_multiaddr));
	retval= setsockopt( m_Socket, IPPROTO_IP, IP_ADD_MEMBERSHIP,(char*)&m_MemberReq, sizeof( m_MemberReq ));
	if( retval == SOCKET_ERROR )
	{
		T_ERR;
	}

	// udp ����
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

		// �۽��ڴ� ��Ƽĳ��Ʈ �׷쿡 ������ �ʿ䰡 ������
		// �̰������� �ٽ� �ޱ� ���ؼ� �����Ͽ���.
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
	// ��Ƽĳ��Ʈ �׷� Ż��
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
