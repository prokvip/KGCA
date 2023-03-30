// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "TCharacter.h"
#include "GameFramework/Actor.h"
#include "Networking.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include <WinSock2.h>
#include "TNetworkManager.generated.h"
#pragma comment(lib, "ws2_32.lib")

#define	MAX_BUFFER		MAX_RECV_SIZE/3

class TWorkThread : public FRunnable
{
private:
	//FRunnableThread* m_pRunnableThread = nullptr;
public:
	TWorkThread(SOCKET sock);
	~TWorkThread();
public:
	FThreadSafeCounter	StopTaskCounter;
	TStreamPacket		m_StreamPacket;	
	FCriticalSection	m_hMutex;
	SOCKET				m_hSocket;
	char 				recvBuffer[MAX_BUFFER];
public:
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;
};

UENUM()
enum class CONNECT_TYPE : uint8
{
	C_NOCONNECT		UMETA(DisplayName = "NoConnect"),
	C_CONNECT		UMETA(DisplayName = "Connect"),
	C_LOGIN			UMETA(DisplayName = "Login"),
	C_LOGOUT		UMETA(DisplayName = "Logout"),	
	C_LOBBYLOGIN	UMETA(DisplayName = "Lobby-Login"),
	C_LOBBYLOGOUT	UMETA(DisplayName = "Lobby-Logout"),
	C_GAMEPLAYING	UMETA(DisplayName = "GamePlaying"),
	C_OK_VERIFICATION UMETA(DisplayName = "OK_VERIFICATION"),
	C_NO_VERIFICATION UMETA(DisplayName = "FAILED_VERIFICATION"),
};

/**
 * 
 */
UCLASS()
class TNETCORE_API UTNetworkManager : public UObject
{
	GENERATED_BODY()
private:
	UTNetworkManager(const FObjectInitializer& ObjectInitializer);
	
private:
	typedef void(UTNetworkManager::*CallFuction)(UPACKET& t);
	std::map<int, CallFuction>		m_fnExecutePacket; 
	tGUID				m_UserGuid;
	SOCKET				m_hSocket;
public:
	virtual void FinishDestroy() override;
	virtual void BeginDestroy() override;
public:
	tGUID				m_tGuidUser;
	tGUID				m_tGuidLogin;
	tGUID				m_tGuidActiveCharacter;
public:
	TSharedPtr<TWorkThread> WorkThread;
	FRunnableThread*	CurrentThread;
	CONNECT_TYPE        ConnectType;
public:
	bool InitNetWorkManager();
	void Destroy();
	bool InitSocket();
	int  CheckReturn(int iRet);
	bool Connect(const char * pszIP, int nPort);
	int  SendPacket(int iType, stringstream& data);
	void CloseSocket();
	bool StartThread();
	void StopThread();
public:
	bool Verification();   // 접속 검증 요청
	bool Verification(tGUID& tGuid);   // 접속 검증 요청
	int  VerificationAck(UPACKET& packet); // 접속 검증 결과 확인 
	bool SendRespawnPlayer(TCharacter& info);
	bool CreateAccount(const FText & Id, const FText & Pw);
	int CreateAccountAck(UPACKET& packet);
	bool Login(const FText & Id, const FText & Pw);
	int LoginAck(UPACKET& packet);
	bool SendZoneEntryReq(TCharacter& info);
	bool SendPlayerInfo(TCharacter& info);
	bool LogoutPlayer(const tGUID& tGuid);
	bool HitPlayer(const tGUID& tGuid);
	bool EnemyHit(const tGUID & Guid);
	bool SendChat(const tGUID& tGuid, const string& Chat);
	bool		SendMoveTo(TCharacter& info, FVector& vTarget);
	TCharacter	RecvMoveTo(UPACKET& packet, FVector& pTarget);
	TCharacter	RecvNpcMove(UPACKET& packet, TCharacter& pDesk);
	TCharacter	RecvNpcAttack(UPACKET& packet, TCharacter& pDesk);
	bool SendEnemyInfo();
private:
	FCriticalSection	m_hNpcMutex;
	TCharacterMap		m_PlayerCharacterList;		
	TCharacterMap		m_GameNpcList;	
public:
	std::map<int, TPACKET_TARGETPOINT_INFO> m_tpMap;
	TCharacterMap&      GetPlayerList() { return m_PlayerCharacterList; };
	TCharacterMap&      GetNpcList() { return m_GameNpcList; };
	TCharacter*         GetCharacter(tGUID tGuid);
public:
	TCharacterMap*	RecvZonePlayersInfo(UPACKET& packet);
	void			RecvZoneInfo(UPACKET& packet);
	tGUID			RecvPlayerLogout(UPACKET& packet);	
	string			RecvMsg(UPACKET& packet);
	TCharacter		RecvEnemyCharacter(UPACKET& packet);
	TCharacter		RecvCharacter(UPACKET& packet);	
	TCharacterMap*	RecvNpcMap(UPACKET& packet);	
	void			SendTargetPointInfo();
};
