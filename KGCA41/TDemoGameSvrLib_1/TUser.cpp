#include "TUser.h"
#include "TServer.h"

RUNTIME_IMPLEMENT(TUser)
TSession::TSession()
{
}
TSession::~TSession()
{
}

//------------------------------------------------------------------------------
TCharacterList& TUser::GetCharacterList()
{
	return m_CharacterList;
}

void	    TUser::SetActiveCharacter(TCharacter* pChar, bool bAlive)
{
	m_pCharacter = pChar;
	if (m_pCharacter)
	{
		m_pCharacter->SetAlive(bAlive);
	}
}
OVERLAPPED2*	TUser::Overlapped( int iType)
{
	OVERLAPPED2* ov = new OVERLAPPED2(iType);
	//ZeroMemory(&m_Overlapped, sizeof(OVERLAPPED2));
	m_Overlapped.m_iFlags = iType;
	ZeroMemory(m_strBuffer, sizeof(char)*MAX_USER_RECV_SIZE);
	m_wsaRecvBuffer.buf = m_strBuffer;
	m_wsaRecvBuffer.len = MAX_USER_RECV_SIZE;
	return ov;
}
OVERLAPPED2*	TUser::OverlappedZero()
{
	OVERLAPPED2* ov = new OVERLAPPED2(OVERLAPPED2::MODE_ZERO_RECV);
	//ZeroMemory(&m_OverlappedZero, sizeof(OVERLAPPED2));
	m_wsaZeroBuffer.buf = nullptr;
	m_wsaZeroBuffer.len = 0;
	//m_OverlappedZero.m_iFlags = OVERLAPPED2::MODE_ZERO_RECV;
	return ov;
}
void TUser::OnRead(size_t len)
{
	//mRecvBuffer.Commit(len);

	/// 패킷 파싱하고 처리
	while (true)
	{
		/// 패킷 헤더 크기 만큼 읽어와보기
		PACKET_HEADER header;
		if (false == mRecvBuffer.Peek((char*)&header, sizeof(PACKET_HEADER)))
			return;

		/// 패킷 완성이 되는가? 
		if (mRecvBuffer.GetStoredSize() < (size_t)header.len)
			return;


		if (header.iotype != 777 || header.len < 0 || header.len > 512)
		{			
			Disconnect();
			return;
		}	

		T_PACKET    add;
		ZeroMemory(&add, sizeof(add));
		mRecvBuffer.Read((char*)&add, header.len);
		add.pUser = this;
		I_Server.AddRecvPacket(add);
	}
}
bool TUser::Dispatch(DWORD deByteSize, LPOVERLAPPED ov)
{
	{
		TLock(this);
		OVERLAPPED2* pOV = reinterpret_cast<OVERLAPPED2*>(ov);
		//OVERLAPPED2* pOV = (OVERLAPPED2*)ov;
		if (pOV->m_iFlags == OVERLAPPED2::MODE_RECV && deByteSize > 0)
		{
			delete pOV;
			m_StreamPacket.Put(m_wsaRecvBuffer.buf, deByteSize, this);
			if (m_StreamPacket.GetPacket(this)==false)
			{
				Disconnect();
				return false;
			}
			//mRecvBuffer.Write(m_wsaRecvBuffer.buf, deByteSize);
			//OnRead(deByteSize);
			
			return WaitForZeroByteReceive();
		}
		if (pOV->m_iFlags != OVERLAPPED2::MODE_ZERO_RECV && deByteSize == 0)
		{
			delete pOV;
			return false;
		}

		if (pOV->m_iFlags == OVERLAPPED2::MODE_ZERO_RECV)
		{
			delete pOV;
			return WaitReceive();
		}		
		if (pOV->m_iFlags == OVERLAPPED2::MODE_SEND)
		{
			delete pOV;
			return true;
		}
		delete pOV;
		return false;
	}
}
bool TUser::WaitForPacketReceive()
{
	int iRet = 0;
	DWORD cbTransferred, dwFlags = 0;
	OVERLAPPED2* pOverlapped = Overlapped(OVERLAPPED2::MODE_RECV);
	//OVERLAPPED2& pOverlapped = Overlapped(OVERLAPPED2::MODE_RECV);
	iRet = WSARecv(m_Socket, &m_wsaRecvBuffer, 1, &cbTransferred, &dwFlags, 
		(LPOVERLAPPED)pOverlapped, NULL);
	if (iRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			return false;
		}
	}	
	if (iRet == 0)
	{
		m_StreamPacket.Put(m_wsaRecvBuffer.buf, cbTransferred, this);
		m_StreamPacket.GetPacket(this);
		return WaitForPacketReceive();
	}
	return true;
}
bool TUser::WaitReceive()
{
	int iRet = 0;
	DWORD cbTransferred, dwFlags = 0;
	OVERLAPPED2* pOverlapped = Overlapped(OVERLAPPED2::MODE_RECV);	
	iRet = WSARecv(m_Socket, &m_wsaRecvBuffer, 1, &cbTransferred, &dwFlags,
				(LPOVERLAPPED)pOverlapped, NULL);
	if (iRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			return false;
		}
	}
	return true;
}
bool TUser::WaitForZeroByteReceive()
{
	int iRet = 0;
	DWORD cbTransferred, flags = 0;
	OVERLAPPED2* pOverlapped = OverlappedZero();
	iRet = WSARecv(m_Socket, &m_wsaZeroBuffer, 1, &cbTransferred, &flags,
		(LPOVERLAPPED)pOverlapped, NULL);
	if (iRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			return false;
		}
	}
	return true;
}
void TUser::Release()
{
	shutdown(m_Socket, SD_BOTH);
	closesocket(m_Socket);
	m_Socket = 0;
}
void TUser::Disconnect()
{
	if (INVALID_SOCKET == m_Socket)
		return;

	shutdown(m_Socket, SD_BOTH);
	/// 즉각 해제
	LINGER lingerOption;
	lingerOption.l_onoff = 1;
	lingerOption.l_linger = 0;
	/// no TCP TIME_WAIT
	if (SOCKET_ERROR == setsockopt(m_Socket, SOL_SOCKET, SO_LINGER, (char*)&lingerOption, sizeof(LINGER)))
	{
		printf_s("[DEBUG] setsockopt linger option error: %d\n", GetLastError());
		return;
	}
	closesocket(m_Socket);
}

TUser::TUser() : mRecvBuffer(MAX_RECV_STREAM_SIZE)
{	
	m_ConnectType = CONNECT_TYPE::C_NOCONNECT;
	m_iType = 0;
}


TUser::~TUser()
{
}

ostream& operator<<(ostream &stream, TUser& info)
{
	TLock sync(&info);
	stream << info.m_tGuid;
	stream << info.m_GuidActiveCharacter;
	stream << info.m_Name << endl;
	stream << info.m_CharacterList.size();

	for (auto& tChar : info.m_CharacterList)
	{
		stream << tChar.first;
		stream << tChar.second;
	}
	return stream;
}

istream& operator>>(istream& stream, TUser& info)
{
	TLock sync(&info);
	int iNumCharacter = 0;
	tGUID tGuid;
	TCharacter tChar;
	info.m_CharacterList.clear();

	stream >> info.m_tGuid;
	stream >> info.m_GuidActiveCharacter;
	stream >> info.m_Name;
	stream >> iNumCharacter;

	for (int i = 0; i < iNumCharacter; i++)
	{
		stream >> tGuid;
		stream >> tChar;
		info.m_CharacterList[tGuid] = tChar;
	}
	return stream;
}