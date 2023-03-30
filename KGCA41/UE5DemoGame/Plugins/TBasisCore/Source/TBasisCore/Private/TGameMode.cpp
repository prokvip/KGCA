// Fill out your copyright notice in the Description page of Project Settings.

#include "TGameMode.h"
#include "TimerManager.h"
//#include "Runtime/Slate/Public/Framework/Commands/InputChord.h" //"SlateCore", "Slate"
#include "Components/InputComponent.h"
#include "UObject/ConstructorHelpers.h"

ATGameMode::ATGameMode() : AGameModeBase()
{
	IsVerification = false;	
	PrimaryActorTick.bCanEverTick = true;
	/*m_fnExecutePacket[PACKET_CHAT_NAME_REQ] = &ATGameMode::ReqChatName;
	m_fnExecutePacket[PACKET_CHAT_NAME_ACK] = &ATGameMode::AckChatName;
	m_fnExecutePacket[PACKET_CHAT_MSG]		= &ATGameMode::Msg;
	m_fnExecutePacket[PACKET_USER_POSITION] = &ATGameMode::MoveAction;
	m_fnExecutePacket[PACKET_STOP_POSITION] = &ATGameMode::MoveAction;*/
}
void ATGameMode::BeginPlay()
{
	//EnableInput(this);
	m_pGameInstance = Cast<UTGameInstance>(GetGameInstance());
	if (m_pGameInstance != nullptr)
	{
		m_pTNet = m_pGameInstance->GetNetworkMgr();
	}

	OnLoginOKDelegate.AddDynamic(this, &ATGameMode::LoginOK);
	OnLoginAckDelegate.BindUFunction(this, "LoginAck");
}
void ATGameMode::Tick(float DeltaSeconds) 
{
	Super::Tick(DeltaSeconds);
	ProcessPacket();
}
void ATGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	m_pGameInstance->DisConnected();
}

bool ATGameMode::Connected(const FString& ip, int iPort)
{
	if (m_pTNet != nullptr)
	{
		if (m_pGameInstance->Connected(ip,iPort))
		{
			//GetWorldTimerManager().ClearTimer(TimerHandle);
			//GetWorldTimerManager().SetTimer(TimerHandle, this, &ATGameMode::ProcessPacket, 1.0f / 60.0f, true);
			return true;
		}
	}
	return false;
}
bool ATGameMode::DisConnected()
{
	GetWorldTimerManager().ClearTimer(TimerHandle);

	//if (GEngine)GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("DisConnected..."));
	if (m_pGameInstance != nullptr)
	{		
		return m_pGameInstance->DisConnected();
	}
	return false;
}
void ATGameMode::ProcessPacket()
{
	//UE_LOG(LogClass, Log, TEXT("ProcessPacket!"));

}
bool ATGameMode::LoginServer(const FText & Id, const FText & Pw)
{
	if (m_pTNet == nullptr || !m_pGameInstance->bIsConnected)
	{
		return false;
	}
	if (Id.IsEmpty() || Pw.IsEmpty() )
	{
		return false;
	}
	if (m_pTNet->Login(Id, Pw))
	{
		return true;
	}
	return false;
}

bool ATGameMode::CreateAccount(const FText & Id, const FText & Pw)
{
	if (m_pTNet == nullptr || !m_pGameInstance->bIsConnected)
	{
		return false;
	}
	if (Id.IsEmpty() || Pw.IsEmpty() )
	{
		return false;
	}
	if (m_pTNet->CreateAccount(Id, Pw))
	{
		return true;
	}
	return false;
}

int ATGameMode::CreateAccountAck_Implementation(int iReturnValue)
{
	return 1;
}

int ATGameMode::LoginAck_Implementation(int retValue)
{
	//UE_LOG(LogClass, Log, TEXT("LoginAck!"));
	OnLoginOKDelegate.Broadcast(retValue);
	//UE_LOG(LogClass, Log, TEXT("success!"));
	IsVerification = true;
	return 1;
}
void ATGameMode::LoginOK(int iReturnValue)
{
	//UE_LOG(LogClass, Log, TEXT("success!"));
}


