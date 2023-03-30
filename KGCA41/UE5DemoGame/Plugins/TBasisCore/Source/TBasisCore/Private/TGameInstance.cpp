// Fill out your copyright notice in the Description page of Project Settings.

#include "TGameInstance.h"
#include "TGameMode.h"

UTGameInstance::UTGameInstance(const FObjectInitializer& ObjInit) : Super(ObjInit) 
{	
	
}
UTNetworkManager *UTGameInstance::GetNetworkMgr()
{
	return	IsValid(m_pTNet) ? m_pTNet : 
		m_pTNet = NewObject<UTNetworkManager>(this,FName("UTNetworkManager"));
}

void UTGameInstance::StartGameInstance()
{
	Super::StartGameInstance();	
	UE_LOG(LogTemp, Warning, TEXT("StartGameInstance, path = %s"), *GetPathName());
}

void UTGameInstance::OnStart()
{
	Super::OnStart();
	//UE_LOG(LogTemp, Warning, TEXT("OnStart, path = %s"), *GetPathName());
}
void UTGameInstance::Init()
{
	Super::Init();
	//Create Console Commands
	FString MyCommandString = "r.SetRes ";
	FString Resolution = "1024x768";
	FString Final = MyCommandString + Resolution;
	GetWorld()->Exec(GetWorld(), *Final);
	int fps = 30;
	IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("t.MaxFPS"));
	CVar->Set(fps);
	//GetOwningPlayer()->ConsoleCommand("StringValue");
	if (m_pTNet == nullptr)
	{
		m_pTNet = NewObject<UTNetworkManager>(this, FName("UTNetworkManager"));
		if (IsValid(m_pTNet))
		{
			m_pTNet->InitNetWorkManager();
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("%s"), TEXT("TGameNetwork Init!"));
}
void UTGameInstance::Shutdown()
{	
	Super::Shutdown();
	//UE_LOG(LogTemp, Warning, TEXT("%s"), TEXT("TGameNetwork Shutdown!"));
	if (IsValid(m_pTNet))
	{
		m_pTNet->Destroy();
	}
	bIsConnected = false;
	
}
bool UTGameInstance::Connected(const FString& ip, int iPort)
{
	if (bIsConnected == false)
	{
		m_pTNet->InitSocket();
		bIsConnected = m_pTNet->Connect(TCHAR_TO_ANSI(*ip), iPort);
		if (bIsConnected)
		{
			//UE_LOG(LogClass, Log, TEXT("IOCP Server connect success!"));
		}
	}
	return true;
}
bool UTGameInstance::DisConnected()
{
	if (m_pTNet != nullptr)
	{		
		m_pTNet->CloseSocket();
		bIsConnected = false;
		return true;
	}		
	return true;
}