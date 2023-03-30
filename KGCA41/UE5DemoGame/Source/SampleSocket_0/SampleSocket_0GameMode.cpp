// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SampleSocket_0GameMode.h"
#include "TBaseCharacter.h"
#include "GamePlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "UObject/UObjectGlobals.h"
#include "Engine/TargetPoint.h" // ATargetPoint
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

ASampleSocket_0GameMode::ASampleSocket_0GameMode() : ATGameMode()
{
	// set default pawn class to our Blueprinted character
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
void ASampleSocket_0GameMode::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogClass, Log, TEXT("ASampleSocket_0GameMode::BeginPlay Start"));
	m_pTNet->ConnectType = CONNECT_TYPE::C_GAMEPLAYING;
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
	m_pPlayerController->OnUserDeadDelegate.AddDynamic(this, &ASampleSocket_0GameMode::OnUserCharacterKill);
	OnCharacterReset();
}
void ASampleSocket_0GameMode::OnCharacterReset()
{	
	ATBaseCharacter* pUserCharacter = Cast<ATBaseCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	check(pUserCharacter);
	pUserCharacter->OnDestroyed.AddDynamic(this, &ASampleSocket_0GameMode::OnCharacterRespawn);
}
void ASampleSocket_0GameMode::OnUserCharacterKill()
{
	APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);
	ATBaseCharacter* pUserCharacter = Cast<ATBaseCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	check(pUserCharacter);
	//CameraAtDeathTransform = pUserCharacter->GetFollowCamera()->GetComponentToWorld();
	//if (pDeadCamera != nullptr) pDeadCamera->Destroy();
	//pDeadCamera = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), CameraAtDeathTransform);
	//if (pDeadCamera != nullptr)
	//{
	//	UCameraComponent* pCameraComponect = pDeadCamera->GetCameraComponent();
	//	pCameraComponect->SetConstraintAspectRatio(false);
	//	OurPlayerController->SetViewTargetWithBlend(pDeadCamera, 0.75f);// , EViewTargetBlendFunction::VTBlend_Linear, 0.0f, true);
	//}	
}
void ASampleSocket_0GameMode::OnCharacterRespawn(AActor* DestroyedActor)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.Name = FName("None");
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ATBaseCharacter* SpawnCharacter = GetWorld()->SpawnActor<ATBaseCharacter>(DefaultPawnClass,	InitSpawnTransform,	SpawnParams);
	check(SpawnCharacter);
	APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);
	OurPlayerController->Possess(SpawnCharacter);
}
void ASampleSocket_0GameMode::SendTargetPointInfo()
{
	if (m_pGameInstance->bIsConnected == false) return;
	
	stringstream SendStream;	
	TPACKET_TARGETPOINT_INFO tp;

	TArray<AActor*> TargetPoint;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), TargetPoint);
	
	int iNumCnt = TargetPoint.Num();
	for (auto& target : TargetPoint)
	{
		ATargetPoint* tpInfo = Cast<ATargetPoint>(target);		
		tp.iType = 0;
		tp.vPosition = tpInfo->GetActorLocation();		
		m_pTNet->m_tpMap[m_pTNet->m_tpMap.size()] = tp;
	}	
	m_pTNet->SendTargetPointInfo();
}

void ASampleSocket_0GameMode::ProcessPacket()
{
	if (m_pTNet == nullptr || m_pTNet->WorkThread.IsValid() == false) return;

	check(m_pPlayerController != nullptr);

	if (m_pTNet->WorkThread->m_hMutex.TryLock())
	{		
		for (auto& Packet : m_pTNet->WorkThread->m_StreamPacket.m_PacketList)
		{	
			switch (Packet.ph.type)
			{			
				case TPACKET_TYPE::PACKET_USER_POSITION:
				{
					FVector  vTarget;
					TCharacter srcChar = m_pTNet->RecvMoveTo(Packet, vTarget);
					m_pPlayerController->RecvMoveTo(srcChar, vTarget);
				}break;
				case TPACKET_TYPE::PACKET_VERIFICATION_ACK:
				{
					if (m_pTNet->VerificationAck(Packet) == 1)
					{
						IsVerification = true;
						m_pPlayerController->SendZoneEntry();
					}
					else
					{
						m_pGameInstance->DisConnected();
					}
					break;				
				}
				case TPACKET_TYPE::PACKET_DEAD_CHARACTER:
				{
					m_pPlayerController->DeadCharacter(m_pTNet->RecvCharacter(Packet));
				}break;
				case TPACKET_TYPE::PACKET_DAMAGE_CHARACTER:
				{
					m_pPlayerController->DamageCharacter(m_pTNet->RecvCharacter(Packet));
				}break;
				case TPACKET_TYPE::PACKET_SPAWN_CHARACTER:
				{
					m_pPlayerController->RespawnCharacter(m_pTNet->RecvCharacter(Packet));
				}break;
				case TPACKET_TYPE::PACKET_LOGOUT_PLAYER:
				{
					m_pPlayerController->RecvPlayerLogout(m_pTNet->RecvPlayerLogout(Packet));
				}
				break;
				case TPACKET_TYPE::PACKET_ZONE_ENTRY_ACK:
				{
					m_pTNet->RecvZoneInfo(Packet);					
					m_pPlayerController->RecvZoneEntry();
				}break;
				case TPACKET_TYPE::PACKET_ZONE_PLAYERS_INFO:
				{
					m_pTNet->RecvZonePlayersInfo(Packet);
				}
				break;
				case TPACKET_TYPE::PACKET_CHAT_MSG:
				{
					m_pPlayerController->RecvMsg(m_pTNet->RecvMsg(Packet));
				}
				break;
				case TPACKET_TYPE::PACKET_ZONE_NEW_PLAYER:
				{
					//RecvZoneNewCharacter.Broadcast(m_pTNet->RecvCharacter(Packet))
					m_pPlayerController->RecvZoneNewCharacter(m_pTNet->RecvCharacter(Packet));
				}
				break;
				case TPACKET_TYPE::PACKET_SPAWN_NPC_LIST:
				{
					m_pTNet->RecvNpcMap(Packet);
					m_pPlayerController->SpawnNPCList();
				}
				break;
				case TPACKET_TYPE::PACKET_SYNC_NPC_LIST:
				{
					m_pPlayerController->UpdateEnemyList(m_pTNet->RecvNpcMap(Packet));
				}
				break;
				case TPACKET_TYPE::PACKET_SPAWN_NPC:
				{
					m_pPlayerController->SpawnEnemyCharacter(m_pTNet->RecvEnemyCharacter(Packet));
				}
				break;
				case TPACKET_TYPE::PACKET_DEAD_NPC:
				{
					m_pPlayerController->DeadEnemyCharacter(m_pTNet->RecvEnemyCharacter(Packet));
				}				
				break;
				case TPACKET_TYPE::PACKET_NPC_TARGETMOVE:
				{
					TCharacter deskChar;
					TCharacter srcChar = m_pTNet->RecvNpcMove(Packet, deskChar);
					m_pPlayerController->MoveEnemyNPC(srcChar, deskChar);					
				}
				break;
				case TPACKET_TYPE::PACKET_ZONE_TARGET_REQ:
				{
					SendTargetPointInfo();					
				}break;
				case TPACKET_TYPE::PACKET_ATTACK_NPC:
				{
					TCharacter deskChar;
					TCharacter srcChar = m_pTNet->RecvNpcAttack(Packet, deskChar);
					m_pPlayerController->AttackEnemyCharacter(srcChar, deskChar);					
				}break;
			}
		}
		m_pTNet->WorkThread->m_StreamPacket.m_PacketList.clear();
		m_pTNet->WorkThread->m_hMutex.Unlock();
	}	
	////UE_LOG(LogClass, Log, TEXT("ProcessPacketMutexMutex-Start!"));
}