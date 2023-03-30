// Fill out your copyright notice in the Description page of Project Settings.

#include "TNetworkManager.h"
#include "Windows/MinWindows.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformAffinity.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"
#include "TProtocol.h"

TWorkThread::TWorkThread(SOCKET sock) :StopTaskCounter(0)//, m_StreamPacket(MAX_BUFFER)
{
	m_hSocket = sock;
	//m_pRunnableThread = FRunnableThread::Create(this, TEXT("TWorkThread"), 0);
}
TWorkThread::~TWorkThread()
{
	StopTaskCounter.Decrement();	
	//delete m_pRunnableThread;
	//m_pRunnableThread = nullptr;
}
bool TWorkThread::Init()
{
	return true;
}
uint32 TWorkThread::Run()
{
	//GEngine->DeferredCommands.Add(TEXT("MYCOMMAND"));
	while (StopTaskCounter.GetValue() == 0)
	{
		////UE_LOG(LogClass, Log, TEXT("RunMutex-Start!"));		
		int iRecvByte = recv(m_hSocket, (CHAR*)&recvBuffer, MAX_BUFFER, 0);
		if (iRecvByte < 0)
		{
			return 1;		
		}
		if (m_hMutex.TryLock())
		{
			m_StreamPacket.Put(recvBuffer, iRecvByte);	
			m_hMutex.Unlock();
		}
						
		////UE_LOG(LogClass, Log, TEXT("RunMutex->End!"));
		FPlatformProcess::Sleep(0.01);
	}
	return 0;
}
void TWorkThread::Exit()
{
	//UE_LOG(LogClass, Log, TEXT("TWorkThread Exit!"));
	shutdown(m_hSocket, SD_BOTH);
	closesocket(m_hSocket);
	if (GEngine)GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Server DisConnected!!"));
}
/*
이 함수 호출전에 논블록킹 소켓을 사용하면 소켓을 먼저 닫아야 된다.
*/
void TWorkThread::Stop()
{
	StopTaskCounter.Increment();		
	/*if (m_pRunnableThread != nullptr)
	{
		m_pRunnableThread->WaitForCompletion();
	}*/
}
bool UTNetworkManager::StartThread()
{
	if (CurrentThread ==nullptr &&  FPlatformProcess::SupportsMultithreading())
	{
		WorkThread = MakeShareable<TWorkThread>(new TWorkThread(m_hSocket));
		CurrentThread = FRunnableThread::Create(WorkThread.Get(), TEXT("UTNetworkManager"));
		return true;
	}
	return false;
}
void UTNetworkManager::StopThread()
{
	// 스레드 종료
	if (WorkThread)
	{
		WorkThread->Stop();
	}	
	if (CurrentThread)
	{
		CurrentThread->WaitForCompletion();
		CurrentThread->Kill();
		delete CurrentThread;
	}	
	CurrentThread = nullptr;	
	WorkThread = NULL;
}
UTNetworkManager::UTNetworkManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WorkThread = nullptr;
	CurrentThread = nullptr;	
}
bool UTNetworkManager::InitSocket()
{		
	//m_hSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	m_hSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_hSocket == INVALID_SOCKET) {
		return false;
	}	
	int option = TRUE;      
	setsockopt(m_hSocket,IPPROTO_TCP,TCP_NODELAY,(const char*)&option,sizeof(option));      
	return true;
}
bool UTNetworkManager::Connect(const char * pszIP, int nPort)
{
	SOCKADDR_IN stServerAddr;
	stServerAddr.sin_family = AF_INET;
	stServerAddr.sin_port = htons(nPort);
	stServerAddr.sin_addr.s_addr = inet_addr(pszIP);
	//int nRet = WSAConnect(m_hSocket, (sockaddr*)&stServerAddr, sizeof(sockaddr),0,0,0,0);
	int nRet = connect(m_hSocket, (sockaddr*)&stServerAddr, sizeof(sockaddr));
	if (nRet == SOCKET_ERROR) {
		return false;
	}
	if (StartThread()==false)
	{
		return false;
	}	
	return true;
}

int UTNetworkManager::CheckReturn(int iRet)
{
	if (iRet == 0)
	{
		return 0;
	}
	if (iRet <= SOCKET_ERROR)
	{
		return -1;
	}
	return iRet;
}
int  UTNetworkManager::SendPacket(int iType, stringstream& data)
{
	if (m_hSocket == INVALID_SOCKET) 
	{
		CloseSocket();
		return 0;
	}
	int sendbytes = 0;
	//if (WorkThread->m_hMutex.TryLock())
	{
		stringstream SendStream = MakePacket(iType, data);
		// 주의 : 직접 SendStream.str().c_str()을 문자열 포인터로 변환하면 안된다.
		const std::string& resultstr = SendStream.str();
		const char* cstr2 = resultstr.c_str();
		int iTotalsize = SendStream.str().length();
		int iSend = 0;		
		do {
			iSend = send(m_hSocket, &cstr2[sendbytes], iTotalsize - sendbytes, 0);
			if (CheckReturn(iSend) <= 0)
			{
				//WorkThread->m_hMutex.Unlock();
				return iSend;
			}
			sendbytes += iSend;		
		} while (sendbytes < iTotalsize);
		//WorkThread->m_hMutex.Unlock();
	}
	return sendbytes;
}
bool UTNetworkManager::CreateAccount(const FText & Id, const FText & Pw)
{
	stringstream data;
	data << TCHAR_TO_UTF8(*Id.ToString()) << endl;
	data << TCHAR_TO_UTF8(*Pw.ToString()) << endl;
	if (SendPacket(TPACKET_TYPE::PACKET_CREATE_ACCOUNT_REQ, data) == -1)
	{
		return false;
	}
	return true;
}
int UTNetworkManager::CreateAccountAck(UPACKET& packet)
{
	string msg(packet.msg, packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	int iRet = -1;
	RecvStream >> iRet;
	RecvStream >> m_UserGuid;
	return iRet;
}
bool UTNetworkManager::Login(const FText & Id, const FText & Pw)
{
	stringstream data;
	data << TCHAR_TO_UTF8(*Id.ToString()) << endl;
	data << TCHAR_TO_UTF8(*Pw.ToString()) << endl;
	if (SendPacket(TPACKET_TYPE::PACKET_LOGIN_REQ, data) == -1)
	{
		return false;
	}
	return true;
}
int UTNetworkManager::LoginAck(UPACKET& packet)
{
	string msg(packet.msg, packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	int iRet = -1;
	int iUserIndex = -1;
	RecvStream >> iRet;
	RecvStream >> m_tGuidLogin;
	RecvStream >> m_tGuidUser;	
	RecvStream >> m_tGuidActiveCharacter;
	

	FGuid fGuid;
	CopyMemory(&fGuid, &m_tGuidLogin, sizeof(fGuid));
	//UE_LOG(LogClass, Log, TEXT("%s"), *fGuid.ToString(EGuidFormats::DigitsWithHyphensInBraces));

	CopyMemory(&fGuid, &m_tGuidUser, sizeof(fGuid));
	//UE_LOG(LogClass, Log, TEXT("%s"), *fGuid.ToString(EGuidFormats::DigitsWithHyphensInBraces));

	CopyMemory(&fGuid, &m_tGuidActiveCharacter, sizeof(fGuid));
	//UE_LOG(LogClass, Log, TEXT("%s"), *fGuid.ToString(EGuidFormats::DigitsWithHyphensInBraces));

	////UE_LOG(LogClass, Log, TEXT("m_tGuidUser->%s"), tostr(m_tGuidUser).c_str());
	////UE_LOG(LogClass, Log, TEXT("m_tGuidLogin->%s"), tostr(m_tGuidLogin).c_str());
	////UE_LOG(LogClass, Log, TEXT("m_tGuidActiveCharacter->%s"), tostr(m_tGuidActiveCharacter).c_str());
	return iRet;
}
bool UTNetworkManager::Verification()
{
	stringstream data;
	data << m_tGuidLogin;
	if (SendPacket(TPACKET_TYPE::PACKET_VERIFICATION_REQ, data) == -1)
	{
		return false;
	}

	//UE_LOG(LogClass, Log, TEXT("LoginGuid->%s"), tostr(m_tGuidLogin).c_str());
	FGuid fGuid;
	CopyMemory(&fGuid, &m_tGuidLogin, sizeof(fGuid));
	//UE_LOG(LogClass, Log, TEXT("%s"), *fGuid.ToString(EGuidFormats::DigitsWithHyphensInBraces));
	return true;
}
bool UTNetworkManager::Verification(tGUID& tGuid)
{
	stringstream data;
	data << tGuid;
	if (SendPacket(TPACKET_TYPE::PACKET_VERIFICATION_REQ, data) == -1)
	{
		return false;
	}
	//UE_LOG(LogClass, Log, TEXT("%s"), tostr(tGuid).c_str());
	return true;
}
int UTNetworkManager::VerificationAck(UPACKET& packet)
{
	string msg(packet.msg, packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	int iRet = -1;

	RecvStream >> iRet;
	if (iRet == 1)
	{
		RecvStream >> m_tGuidUser;
		RecvStream >> m_tGuidActiveCharacter;		
	}
	return iRet;
}
bool UTNetworkManager::SendZoneEntryReq(TCharacter& info)
{
	stringstream SendStream;
	SendStream << info;
	if (SendPacket(TPACKET_TYPE::PACKET_ZONE_ENTRY_REQ, SendStream) == -1)
	{
		return false;
	}
	return true;
}
bool UTNetworkManager::SendPlayerInfo(TCharacter& info)
{
	// 캐릭터 정보 직렬화
	stringstream SendStream;
	SendStream << info;

	//stringstream make = MakePacket(TPACKET_TYPE::PACKET_SYNC_CHARACTER, SendStream);
	//
	//std::string data = make.str();
	//string msg( &data[PACKET_HEADER_SIZE], make.str().length() - PACKET_HEADER_SIZE);
	//stringstream RecvStream(msg);
	//TCharacter tChar;
	//RecvStream >> tChar;


	int iSize = sizeof(TPACKET_CHARACTER_INFO);
	if (SendPacket(TPACKET_TYPE::PACKET_SYNC_CHARACTER, SendStream) == -1)
	{
		return false;
	}
	return true;
}
bool UTNetworkManager::SendMoveTo(TCharacter& info, FVector& vTarget)
{
	// 캐릭터 정보 직렬화
	stringstream SendStream;
 	SendStream << info;
	SendStream << vTarget;
	if (SendPacket(TPACKET_TYPE::PACKET_USER_POSITION, SendStream) == -1)
	{
		return false;
	}
	return true;
}
bool UTNetworkManager::SendRespawnPlayer(TCharacter& info)
{
	// 캐릭터 정보 직렬화
	stringstream SendStream;
	SendStream << info;
	if (SendPacket(TPACKET_TYPE::PACKET_SPAWN_CHARACTER, SendStream) == -1)
	{
		return false;
	}
	return true;
}
tGUID UTNetworkManager::RecvPlayerLogout(UPACKET& packet)
{
	string msg(packet.msg, packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	tGUID tGuid;
	RecvStream >> tGuid;
	m_PlayerCharacterList.tCharMap.erase(tGuid);
	return tGuid;	
}
void UTNetworkManager::RecvZoneInfo(UPACKET& packet)
{
	m_PlayerCharacterList.tCharMap.clear();
	m_GameNpcList.tCharMap.clear();

	string msg(packet.msg, packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	RecvStream >> m_PlayerCharacterList;
	RecvStream >> m_GameNpcList;	
}
TCharacterMap * UTNetworkManager::RecvZonePlayersInfo(UPACKET& packet)
{
	string msg(packet.msg, packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	RecvStream >> m_PlayerCharacterList;
	return &m_PlayerCharacterList;
}
string UTNetworkManager::RecvMsg(UPACKET& packet)
{
	string msg(packet.msg, packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	tGUID tGuid;
	string recvMsg;
	RecvStream >> tGuid;
	RecvStream >> recvMsg;
	std::replace(recvMsg.begin(), recvMsg.end(), '_', ' ');
	return recvMsg;
}
TCharacter UTNetworkManager::RecvCharacter(UPACKET& packet)
{
	string msg(packet.msg, packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	// 새 플레이어 정보를 얻어 반환
	TCharacter tChar;
	RecvStream >> tChar;
	return tChar;
}
TCharacterMap * UTNetworkManager::RecvNpcMap(UPACKET& packet)
{
	string msg(packet.msg, packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	if (m_hNpcMutex.TryLock())
	{	
		RecvStream >> m_GameNpcList;
		m_hNpcMutex.Unlock();
	}
	return &m_GameNpcList;
}
bool UTNetworkManager::SendEnemyInfo()
{	
	stringstream SendStream;
	if (m_hNpcMutex.TryLock())
	{
		SendStream << m_GameNpcList;
		m_hNpcMutex.Unlock();
	}
	if (SendPacket(TPACKET_TYPE::PACKET_SYNC_NPC_LIST, SendStream) == -1)
	{
		return false;
	}
	return true;
}
TCharacter UTNetworkManager::RecvEnemyCharacter(UPACKET& packet)
{
	string msg(packet.msg, packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	TCharacter	npcChar;
	RecvStream >> npcChar;
	return npcChar;
}
TCharacter UTNetworkManager::RecvMoveTo(UPACKET& packet, FVector& pTarget)
{
	string msg(packet.msg, packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	TCharacter	npcChar;
	RecvStream >> npcChar;
	RecvStream >> pTarget;
	return npcChar;
}
TCharacter UTNetworkManager::RecvNpcMove(UPACKET& packet, TCharacter& pDesk)
{
	string msg(packet.msg, packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	TCharacter	npcChar;
	RecvStream >> npcChar;
	RecvStream >> pDesk;
	return npcChar;
}
TCharacter UTNetworkManager::RecvNpcAttack(UPACKET& packet, TCharacter& pDesk)
{
	string msg(packet.msg, packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	TCharacter	npcChar;
	RecvStream >> npcChar;
	RecvStream >> pDesk;
	return npcChar;
}
bool UTNetworkManager::LogoutPlayer(const tGUID& SessionId)
{
	stringstream SendStream;
	SendStream << SessionId;
	if (SendPacket(TPACKET_TYPE::PACKET_LOGOUT_REQ, SendStream) == -1)
	{
		return false;
	}
	shutdown(m_hSocket, SD_BOTH);
	closesocket(m_hSocket);
	return true;
}
bool UTNetworkManager::HitPlayer(const tGUID& SessionId)
{
	// 서버에게 데미지를 준 캐릭터 정보 전송
	stringstream SendStream;
	SendStream << SessionId;

	if (SendPacket(TPACKET_TYPE::PACKET_HIT_CHARACTER, SendStream) == -1)
	{
		return false;
	}
	return true;
}
bool UTNetworkManager::EnemyHit(const tGUID& tGUID)
{
	// 서버에게 데미지를 준 Enemy 정보 전송
	stringstream SendStream;
	SendStream << tGUID;
	if (SendPacket(TPACKET_TYPE::PACKET_HIT_NPC, SendStream) == -1)
	{
		return false;
	}
	return true;
}
bool UTNetworkManager::SendChat(const tGUID& SessionId, const string & Chat)
{
	stringstream SendStream;
	SendStream << SessionId;
	SendStream << Chat;
	if (SendPacket(TPACKET_TYPE::PACKET_CHAT_MSG, SendStream) == -1)
	{
		return false;
	}
	return true;
}
void UTNetworkManager::SendTargetPointInfo()
{
	stringstream SendStream;
	TPACKET_TARGETPOINT_INFO tp;

	int iNumCnt = m_tpMap.size();
	tBlockWrite<int>(SendStream, iNumCnt);

	for (auto& target : m_tpMap)
	{
		tp.iType = target.second.iType;
		tp.vPosition = target.second.vPosition;
		tBlockWrite<TPACKET_TARGETPOINT_INFO>(SendStream, tp);
	}
	SendPacket(TPACKET_TYPE::PACKET_ZONE_TARGET_ACK, SendStream);
	if (GEngine)GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("TPACKET_TYPE::PACKET_ZONE_TARGET_ACK"));
	//UE_LOG(LogClass, Log, TEXT("%s"), "TPACKET_TYPE::PACKET_ZONE_TARGET_ACK");
}
bool UTNetworkManager::InitNetWorkManager()
{
	WSADATA wsaData;
	int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (nRet != 0)
	{
		return false;
	}
	return true;
}
void  UTNetworkManager::FinishDestroy()
{
	Super::FinishDestroy();
	//UE_LOG(LogClass, Log, TEXT("FinisyDestory->End!"));
}
void  UTNetworkManager::BeginDestroy()
{
	Super::BeginDestroy();
	//UE_LOG(LogClass, Log, TEXT("BeginDestory->End!"));
}
void UTNetworkManager::CloseSocket()
{
	shutdown(m_hSocket, SD_BOTH);
	closesocket(m_hSocket);
	StopThread();
	delete CurrentThread;
	CurrentThread = nullptr;
	WorkThread = nullptr;
}
void UTNetworkManager::Destroy()
{	
	CloseSocket();
	WSACleanup();
	//m_Instance = nullptr;
	//MarkPendingKill() 강제소멸
	//GetWorld()->ForceGarbageCollection(true);
}
TCharacter*  UTNetworkManager::GetCharacter(tGUID tGuid)
{
	std::map<tGUID, TCharacter, GUIDComparer>::iterator player;
	player = GetPlayerList().tCharMap.find(tGuid);
	if (player != GetPlayerList().tCharMap.end())
	{
		return &player->second;
	}
	return nullptr;
}

