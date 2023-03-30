// Fill out your copyright notice in the Description page of Project Settings.
// 멀티 언리얼 엔진 실행으로 테스트시 필요한 사항
// 편집->에디터개인설정->성능(Performance)->Use Less CPU when in Background = false 
#include "LoginGameMode.h"
#include "GamePlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

ALoginGameMode::ALoginGameMode() : ATGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassBPClass(TEXT("/Game/BP_GamePlayerController"));
	if (PlayerControllerClassBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerClassBPClass.Class;
	}
}

void ALoginGameMode::BeginPlay()
{
	Super::BeginPlay();
	m_pTNet->ConnectType = CONNECT_TYPE::C_LOGIN;

	if (DefaultWidgetClass != nullptr)
	{
		DefalultWidget = CreateWidget<UUserWidget>(GetWorld(), DefaultWidgetClass);
		if (DefalultWidget != nullptr)
		{
			DefalultWidget->AddToViewport();
		}
	}
	Connected(TEXT("127.0.0.1"), 10000);		
}

void ALoginGameMode::ProcessPacket()
{
	if (m_pTNet == nullptr || m_pTNet->CurrentThread == nullptr ||  m_pTNet->WorkThread.Get() == nullptr ) return;
	if (m_pTNet->WorkThread->m_hMutex.TryLock())
	{
		for (auto& Packet : m_pTNet->WorkThread->m_StreamPacket.m_PacketList)
		{
			string msg(Packet.msg, Packet.ph.len - PACKET_HEADER_SIZE);
			stringstream RecvStream(msg);

			switch (Packet.ph.type)
			{
				case TPACKET_TYPE::PACKET_CREATE_ACCOUNT_ACK:
				{
					m_pTNet->CreateAccountAck(Packet);
				}break;
				case TPACKET_TYPE::PACKET_LOGIN_ACK:
				{
					OnLoginAckDelegate.Execute(m_pTNet->LoginAck(Packet));
				}break;			
			}
		}
		m_pTNet->WorkThread->m_StreamPacket.m_PacketList.clear();
		m_pTNet->WorkThread->m_hMutex.Unlock();
	}	
}