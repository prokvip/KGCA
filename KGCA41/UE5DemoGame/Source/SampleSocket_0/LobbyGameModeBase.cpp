// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameModeBase.h"
#include "GamePlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "ChatWidget.h"
#include "Blueprint/UserWidget.h"

ALobbyGameModeBase::ALobbyGameModeBase() : ATGameMode()
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

void ALobbyGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	m_pTNet->ConnectType = CONNECT_TYPE::C_LOBBYLOGIN;

	if (DefaultWidgetClass != nullptr)
	{
		DefalultWidget = CreateWidget<UUserWidget>(GetWorld(), DefaultWidgetClass);
		if (DefalultWidget != nullptr)
		{
			DefalultWidget->AddToViewport();
		}
	}
	m_pPlayerController = Cast<AGamePlayerController>(GetWorld()->GetFirstPlayerController());
	
	if (Connected(TEXT("127.0.0.1"), 10000))
	{
		m_pTNet->Verification();
	}
}

void ALobbyGameModeBase::ProcessPacket()
{
	if (m_pTNet == nullptr || m_pTNet->WorkThread.IsValid() == false) return;

	if (m_pTNet->WorkThread->m_hMutex.TryLock())
	{
		for (auto& Packet : m_pTNet->WorkThread->m_StreamPacket.m_PacketList)
		{
			string msg(Packet.msg, Packet.ph.len - PACKET_HEADER_SIZE);
			stringstream RecvStream(msg);

			switch (Packet.ph.type)
			{
				case TPACKET_TYPE::PACKET_VERIFICATION_ACK:
				{
					if (m_pTNet->VerificationAck(Packet) == 1)
					{
						IsVerification = true;						
					}
					else
					{
						IsVerification = false;
					}					
					break;
				}
				case TPACKET_TYPE::PACKET_CHAT_MSG:
				{
					UpdateChat(m_pTNet->RecvMsg(Packet));
				}
				break;
			}
		}
		m_pTNet->WorkThread->m_StreamPacket.m_PacketList.clear();
		m_pTNet->WorkThread->m_hMutex.Unlock();
	}	
}
void ALobbyGameModeBase::ChatToServer(FString Text)
{
	//UE_LOG(LogClass, Log, TEXT("%s"), *Text);
	m_pTNet->SendChat(m_pTNet->m_tGuidUser, TCHAR_TO_UTF8(*Text));
}
void ALobbyGameModeBase::UpdateChat(const string chat)
{
	UChatWidget* temp = Cast<UChatWidget>(pChatWiz);
	if (temp != nullptr)
	{
		//UE_LOG(LogClass, Log, TEXT("Casting"));
		//temp->UpdateMsg(FText::FromString(*FString((chat->c_str()))));
		temp->UpdateMsg(FText::FromString(UTF8_TO_TCHAR(chat.c_str())));
	}
}