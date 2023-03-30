// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetGuidLibrary.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "ChatWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/UObjectGlobals.h"
#include "TGameMode.h"
#include "TNpc.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "SampleSocket_0GameMode.h"

ATBaseCharacter* AGamePlayerController::GetCharacter()
{
	return m_pCharacter;
}
void AGamePlayerController::OnConstruction(const FTransform &Transform)
{
	Super::OnConstruction(Transform);
	// Create and register Sprint Component
	InputComponent = NewObject<UInputComponent>(this, UInputComponent::StaticClass(), TEXT("UInputComponent"));
	check(InputComponent);
}
AGamePlayerController::AGamePlayerController()
{
	m_iNumEnemy = 0;
	m_iNumPlayers = 0;
	m_bGameStart = false;
	m_pGameInstance = nullptr;
	PrimaryActorTick.bCanEverTick = true;
}
AGamePlayerController::~AGamePlayerController()
{
}
void AGamePlayerController::ShowFrameRate()
{
	GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Yellow, "FPS: " + FString::FromInt(FPS) + " ms: " + FString::SanitizeFloat(FApp::GetDeltaTime() * 1000));
	FPS = 0;
}
void AGamePlayerController::SetupInputComponent()
{
	EnableInput(this);
	Super::SetupInputComponent();
	InputComponent->BindKey(EKeys::Escape, IE_Released, this, &AGamePlayerController::EscKeyToDisConnected);
}
void AGamePlayerController::EscKeyToDisConnected()
{
	if (m_pGameInstance != nullptr)
	{
		m_pGameInstance->DisConnected();
	}
}
void AGamePlayerController::ChatToServer(FString Text)
{
	//UE_LOG(LogClass, Log, TEXT("%s"), *Text);
	m_pTNet->SendChat(m_tGuid, TCHAR_TO_UTF8(*Text));
}
FGuid AGamePlayerController::GetSessionId()
{
	FGuid guid;
	CopyMemory(&guid, &m_tGuid, sizeof(tGUID));
	return guid;
}
void AGamePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	FPS++;
	if (!m_pGameInstance->bIsConnected || m_bGameStart==false) return;
	// 월드 동기화
	UpdateWorldInfo();
}
TCharacter AGamePlayerController::GetCharacterInfo()
{
	check(m_pCharacter);
	TCharacter tChar;
	if (m_pTNet->GetPlayerList().tCharMap.size() > 0)
	{
		std::map<tGUID, TCharacter, GUIDComparer>::iterator player;
		player = m_pTNet->GetPlayerList().tCharMap.find(m_tGuid);
		if (player != m_pTNet->GetPlayerList().tCharMap.end())
		{
			tChar = player->second;
		}
	}
	tChar.Set(	m_pCharacter->GetActorLocation(), 
				m_pCharacter->GetActorRotation(),
				m_pCharacter->GetVelocity());	
	return  tChar;
}
//void AGamePlayerController::PostInitializeComponents()
//{
//	Super::PostInitializeComponents();
//	createCamera();
//}
//void AGamePlayerController::createCamera() {
//	this->camera = GetWorld()->SpawnActor<Camera>();
//	this->SetViewTargetWithBlend(camera, 0.0f, EViewTargetBlendFunction::VTBlend_Linear, 0.0f, true);
//	camera->SetOwner(this);
//}
void AGamePlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	m_pCharacter = Cast< ATBaseCharacter>(aPawn);
	if (m_pGameInstance==nullptr || m_pGameInstance->bIsConnected == false) return;
	TCharacter tChar = GetCharacterInfo();
	m_pTNet->SendRespawnPlayer(tChar);
	
	ASampleSocket_0GameMode* pGameMode = Cast<ASampleSocket_0GameMode>(GetWorld()->GetAuthGameMode());
	check(pGameMode);
	pGameMode->OnCharacterReset();
	if (GetViewTarget() != nullptr)
	{
		SetViewTargetWithBlend(GetViewTarget(), 1.0f, EViewTargetBlendFunction::VTBlend_Linear, 0.0f, true);
	}
}
void AGamePlayerController::OnUnPossess()
{
	Super::OnUnPossess();
	UE_LOG(LogClass, Log, TEXT("%s"), TEXT("OnUnPossess"));
}
void AGamePlayerController::RespawnCharacter(TCharacter tChar)
{
	if (GEngine)GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("RespawnCharacter"));
	if (m_pTNet->GetPlayerList().tCharMap.size() > 0)
	{
		std::map<tGUID, TCharacter, GUIDComparer>::iterator player;
		player = m_pTNet->GetPlayerList().tCharMap.find(tChar.GetGuid());
		if (player != m_pTNet->GetPlayerList().tCharMap.end())
		{
			player->second = tChar;
		}
		if (tChar.GetGuid() == m_tGuid)
		{
			isDeadCharacter = false;
			//if (GEngine)GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, tChar.GetAlive() ? TEXT("alive") : TEXT("dead"));
		}
		else
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator=GetInstigator();
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			FGuid fGuid;
			tGUID tGuid = tChar.GetGuid();
			CopyMemory(&fGuid, &tGuid, sizeof(fGuid));
			SpawnParams.Name = FName(*fGuid.ToString());
			ATBaseCharacter* SpawnCharacter = 
				GetWorld()->SpawnActor<ATBaseCharacter>(UserCharacterClass,
				tChar.GetPos(),
				tChar.GetRotation(),
				SpawnParams);
			SpawnCharacter->SpawnDefaultController();
			SpawnCharacter->m_tGuidACharacter = tChar.GetGuid();
		}
		return;
	}	
}
void AGamePlayerController::SendZoneEntry()
{
	if (m_pTNet->ConnectType == CONNECT_TYPE::C_GAMEPLAYING)
	{
		TCharacter tChar = GetCharacterInfo();
		tChar.SetGuid(m_pTNet->m_tGuidActiveCharacter);
		m_pCharacter->m_tGuidACharacter = m_tGuid = m_pTNet->m_tGuidActiveCharacter;
		m_pTNet->SendZoneEntryReq(tChar);
		//UE_LOG(LogClass, Log, TEXT("BeginPlay End"));
	}
}
void AGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogClass, Log, TEXT("AGamePlayerController::BeginPlay Start"));
	PlayerCameraManager->StartCameraFade(1, 0, 1, FLinearColor(1, 0, 0), false, true);

	FTimerHandle FPSTimerHandle;
	GetWorldTimerManager().SetTimer(FPSTimerHandle, this, &AGamePlayerController::ShowFrameRate, 1.0f, true);


	m_pGameInstance = Cast<UTGameInstance>(GetGameInstance());
	if (m_pGameInstance != nullptr)
	{
		m_pTNet = m_pGameInstance->GetNetworkMgr();
	}

	// 캐릭터 등록
	m_pCharacter = Cast<ATBaseCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (m_pCharacter == nullptr)
	{
		return;
	}	
	// 스폰 변환행열 저장
	ATGameMode * pGameMode = Cast<ATGameMode>(GetWorld()->GetAuthGameMode());
	pGameMode->InitSpawnTransform = m_pCharacter->GetActorTransform();
	OnUserDeadDelegate.AddDynamic(this, &AGamePlayerController::OnUserCharacterKill);

	isDeadCharacter = false;
}
AActor * AGamePlayerController::FindActorBySessionId(TArray<AActor*> ActorArray, const tGUID & SessionId)
{
	for (const auto& Actor : ActorArray)
	{
		ATBaseCharacter * swc = Cast<ATBaseCharacter>(Actor);
		if (swc && swc->m_tGuidACharacter == m_tGuid)
			return Actor;
	}
	return nullptr;
}
void AGamePlayerController::HitCharacter(const FGuid & Guid)
{
	//UE_LOG(LogClass, Log, TEXT("Player Hit Called %s"), *Guid.ToString());
	tGUID guid;
	CopyMemory(&guid, &Guid, sizeof(FGuid));
	m_pTNet->HitPlayer(guid);
}
void AGamePlayerController::EnemyHit(tGUID & tGuid)
{	
	m_pTNet->EnemyHit(tGuid);
}

void AGamePlayerController::RecvZoneEntry()
{	
	for (TActorIterator<ATNpc> npcIter(GetWorld()); npcIter; ++npcIter)
	{
		ATNpc* npc = (*npcIter);
		npc->Destroy();
	}
	for (TActorIterator<ATBaseCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ATBaseCharacter* pc = (*ActorItr);
		if (pc && pc->m_tGuidACharacter != m_tGuid)
		{
			pc->Destroy();
		}
	}

	SpawnPCList();
	SpawnNPCList();

	m_bGameStart = true;
	if (GetWorldTimerManager().IsTimerActive(SendPlayerInfoHandle)==false)
	{
		GetWorldTimerManager().SetTimer(SendPlayerInfoHandle, this, &AGamePlayerController::SendPlayerInfo, 0.033f, true);
	}
}
// 존 집입시에 한번만 유저케릭터를 스폰한다.
void AGamePlayerController::SpawnPCList()
{
	if (m_pTNet != nullptr)
	{
		UWorld* const world = GetWorld();	
		if (world == nullptr )
		{
			return;
		}
		m_iNumPlayers = m_pTNet->GetPlayerList().tCharMap.size();
		std::map<tGUID, TCharacter, GUIDComparer>::iterator player;
		//for (auto & player : m_pTNet->GetPlayerList().tCharMap)
		for (player = m_pTNet->GetPlayerList().tCharMap.begin(); player != m_pTNet->GetPlayerList().tCharMap.end(); player++)
		{
			if (player->first == m_tGuid || !player->second.GetAlive())
			{
				continue;
			}

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			FGuid fGuid;
			tGUID tGuid = player->second.GetGuid();
			CopyMemory(&fGuid, &tGuid, sizeof(fGuid));
			SpawnParams.Name = FName(*fGuid.ToString());

			ATBaseCharacter* SpawnCharacter = world->SpawnActor<ATBaseCharacter>(UserCharacterClass,
				player->second.GetPos(),
				player->second.GetRotation(),
				SpawnParams);
			if (SpawnCharacter != nullptr)
			{
				SpawnCharacter->SpawnDefaultController();
				SpawnCharacter->m_tGuidACharacter = player->second.GetGuid();
			}
		}
	}
}
void AGamePlayerController::SpawnNPCList()
{
	if (m_pTNet != nullptr)
	{
		UWorld* const world = GetWorld();		
		if (world == nullptr )
		{
			return;
		}		
		
		for (auto& data : m_pTNet->GetNpcList().tCharMap)
		{
			TCharacter& npc = data.second;
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			SpawnParams.SpawnCollisionHandlingOverride =ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			FGuid fGuid;
			tGUID tGuid = npc.GetGuid();
			CopyMemory(&fGuid, &tGuid, sizeof(fGuid));
			SpawnParams.Name = FName(*fGuid.ToString());

			const FVector pos = npc.GetPos();
			ATNpc* SpawnEnemyCharacter = world->SpawnActor<ATNpc>(NpcCharacterClass,
				pos,
				FRotator::ZeroRotator,
				SpawnParams);
			if (SpawnEnemyCharacter)
			{
				SpawnEnemyCharacter->SpawnDefaultController();
				SpawnEnemyCharacter->m_tGuid = npc.GetGuid();
				SpawnEnemyCharacter->m_fHealth = npc.GetHealth();
				//if (GEngine)GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, npc.GetPos().ToString());
			}
		}
	}
}
void AGamePlayerController::RecvMsg(const string msg)
{
	UChatWidget* temp = Cast<UChatWidget>(DefalultWidget);
	if (temp != nullptr)
	{
		//UE_LOG(LogClass, Log, TEXT("Casting"));
		temp->UpdateMsg(FText::FromString(*FString(msg.c_str())));
	}
}
void AGamePlayerController::SendPlayerInfo()
{
	if (m_pGameInstance->bIsConnected == false) return;
	TCharacter tChar = GetCharacterInfo();
	if (isDeadCharacter == false)
	{		
		m_pTNet->SendPlayerInfo(tChar);		
	}
	if (tChar.GetType() == 1 && m_pTNet->GetNpcList().tCharMap.size() > 0)
	{
		SendEnemyInfo();		
	}
	//if (GEngine)GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, tChar.GetPos().ToString());
}
void AGamePlayerController::SendEnemyInfo()
{
	if (m_pGameInstance->bIsConnected == false) return;
	UWorld* const world = GetWorld();
	if (world)
	{
		TArray<AActor*> npcArray;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATNpc::StaticClass(), npcArray);
		for (auto actor : npcArray)
		{
			ATNpc * pNpc = Cast<ATNpc>(actor);
			if (pNpc)
			{
				TCharacter& tChar = m_pTNet->GetNpcList().tCharMap[pNpc->m_tGuid];
				tChar.SetPos(pNpc->GetActorLocation());
				tChar.SetRotation(pNpc->GetActorRotation());
				tChar.SetVelocity(pNpc->GetVelocity());
				//if (GEngine)GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, tChar.GetPos().ToString());
			}
		}
	}
	m_pTNet->SendEnemyInfo();
}
void AGamePlayerController::RecvPlayerLogout(tGUID tGuid)
{
	TArray<AActor*> SpawnedCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATBaseCharacter::StaticClass(), SpawnedCharacters);
	for (auto& Character : SpawnedCharacters)
	{
		ATBaseCharacter * Player = Cast<ATBaseCharacter>(Character);
		if ( Player && Player->m_tGuidACharacter == tGuid)
		{
			UE_LOG(LogClass, Log, TEXT("Player Logout"));
			Player->Destroy();
		}
	}
	UE_LOG(LogClass, Log, TEXT("Player Logout Not! find"));
}
bool AGamePlayerController::UpdateWorldInfo()
{
	UWorld* const world = GetWorld();
	if (world == nullptr )
	{
		return false;
	}
	UpdatePlayerInfo(m_pTNet->GetPlayerList().tCharMap[m_tGuid]);
	TArray<AActor*> SpawnedCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATBaseCharacter::StaticClass(), SpawnedCharacters);	
	// 플레이어 케릭터는 제외한다.
	check(m_pCharacter);
	SpawnedCharacters.Remove(m_pCharacter);

	ATBaseCharacter * userChar = nullptr;
	for (auto& Character : SpawnedCharacters)
	{
		userChar = Cast<ATBaseCharacter>(Character);
		TCharacter * info = &m_pTNet->GetPlayerList().tCharMap[userChar->m_tGuidACharacter];
		
		if (info && info->GetAlive())
		{						
			userChar->SetActorRotation(info->GetRotation());			
			userChar->AddMovementInput(info->GetVelocity());
			userChar->SetActorLocation(info->GetPos());

			//FVector pos = userChar->GetActorLocation();
			//pos.Y = info->GetPos().Y;
			//userChar->SetActorLocation(pos);// Jump 적용
		}		
	}	
	return true;
}
void AGamePlayerController::UpdatePlayerInfo(const TCharacter & info)
{
	auto Player = Cast<ATBaseCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (!Player || isDeadCharacter )
	{
		return;
	}
}
void AGamePlayerController::OnUserCharacterKill()
{
	check(m_pCharacter);
	FTransform transform(m_pCharacter->GetActorLocation());
	if (DestroyEmiiter != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestroyEmiiter, transform, true);
	}	
	m_pCharacter->DeadAnimation();	

	FTimerHandle FPSTimerHandle;
	GetWorldTimerManager().SetTimer(FPSTimerHandle, this, &AGamePlayerController::DestroyCharacter, 3.0f, false);
}
void AGamePlayerController::DestroyCharacter()
{
	ATGameMode * pGameMode = Cast<ATGameMode>(GetWorld()->GetAuthGameMode());
	ATBaseCharacter* Player = Cast<ATBaseCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	
	//check(Player);
	//Player->Destroy();
	/*if (DefalultWidget != nullptr)
	{
		DefalultWidget->RemoveFromParent();
		GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
		if (GameOverWidget != nullptr)
		{
			GameOverWidget->AddToViewport();
		}
	}*/
}
void AGamePlayerController::DeadCharacter(TCharacter tChar)
{
	if (m_pTNet->GetPlayerList().tCharMap.size() > 0)
	{
		std::map<tGUID, TCharacter, GUIDComparer>::iterator player;
		player = m_pTNet->GetPlayerList().tCharMap.find(tChar.GetGuid());
		if (player != m_pTNet->GetPlayerList().tCharMap.end())
		{
			player->second = tChar;
		}
		if (tChar.GetGuid() == m_tGuid)
		{
			isDeadCharacter = true;
			OnUserDeadDelegate.Broadcast();
		}
		else
		{
			TArray<AActor*> SpawnedCharacters;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATBaseCharacter::StaticClass(), SpawnedCharacters);
			check(m_pCharacter);
			SpawnedCharacters.Remove(m_pCharacter);

			ATBaseCharacter * userChar = nullptr;
			for (auto& Character : SpawnedCharacters)
			{
				userChar = Cast<ATBaseCharacter>(Character);
				//TCharacter * info = &m_pTNet->GetPlayerList().tCharMap[userChar->tGuid];
				if (userChar->m_tGuidACharacter == tChar.GetGuid())
				{
					FTransform transform(userChar->GetActorLocation());
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestroyEmiiter, transform, true);
					userChar->DeadAnimation();
					break;
				}
			}
			//UE_LOG(LogClass, Log, TEXT("Other Player Dead."));
		}
	}
}
void AGamePlayerController::DamageCharacter(TCharacter tChar)
{
	if (m_pTNet->GetPlayerList().tCharMap.size() > 0)
	{
		TArray<AActor*> SpawnedCharacters;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATBaseCharacter::StaticClass(), SpawnedCharacters);
		ATBaseCharacter * userChar = nullptr;
		for (auto& Character : SpawnedCharacters)
		{
			userChar = Cast<ATBaseCharacter>(Character);
			if (userChar->m_tGuidACharacter == tChar.GetGuid())
			{				
				userChar->m_fHealth = tChar.GetHealth();
				userChar->DamageAnimation();
				break;
			}
		}
		//UE_LOG(LogClass, Log, TEXT("Player Damage."));		
	}
}
// 존에 신규로 진입한 유저케릭터를 스폰한다.
void AGamePlayerController::RecvZoneNewCharacter(TCharacter tCharacter)
{
	UWorld* const world = GetWorld();
	if (tCharacter.GetGuid() == m_tGuid)
	{
		return;
	}

	// 새로운 플레이어를 필드에 스폰
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	FGuid fGuid;
	tGUID tGuid = tCharacter.GetGuid();
	CopyMemory(&fGuid, &tGuid, sizeof(FGuid));
	SpawnParams.Name = FName(*fGuid.ToString());
	//SpawnParams.Name = FName(*FString(to_string(NewPlayer->tGuid).c_str()));

	ATBaseCharacter* SpawnCharacter = world->SpawnActor<ATBaseCharacter>(UserCharacterClass, 
		tCharacter.GetPos(),
		tCharacter.GetRotation(),
		SpawnParams);
	SpawnCharacter->SpawnDefaultController();
	SpawnCharacter->m_tGuidACharacter = tCharacter.GetGuid();
	SpawnCharacter->m_fHealth = tCharacter.GetHealth();

	m_pTNet->GetPlayerList().tCharMap[tCharacter.GetGuid()] = tCharacter;
	//UE_LOG(LogClass, Log, TEXT("other player spawned."));
}
void AGamePlayerController::UpdateEnemyList(TCharacterMap * pNpcMap)
{
	// 이동 네이게이션 메쉬, 공격, 데미지, 데드 패킷 사용하기 때문에 주석처리함.
	//UWorld* const world = GetWorld();
	//if (world)
	//{		
	//	TArray<AActor*> npcArray;
	//	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATNpc::StaticClass(), npcArray);
	//	for (auto actor : npcArray)
	//	{
	//		ATNpc * pNpc = Cast<ATNpc>(actor);
	//		if (pNpc)
	//		{
	//			TCharacter* pNpcChar = &m_pTNet->GetNpcList().tCharMap[pNpc->m_tGuid];			
	//			if (pNpcChar && pNpcChar->GetAlive())
	//			{
	//				pNpc->SetActorRotation(pNpcChar->GetRotation());
	//				pNpc->AddMovementInput(pNpcChar->GetVelocity());
	//				FVector pos = pNpc->GetActorLocation();
	//				pos.Y = pNpcChar->GetPos().Y;
	//				pNpc->SetActorLocation(pos);// Jump 적용
	//			}
	//		}
	//	}		
	//}
}
void AGamePlayerController::SpawnEnemyCharacter(TCharacter npc)
{
	UWorld* const world = GetWorld();
	if (world)
	{
		FVector SpawnLocation = npc.GetPos();
		FRotator SpawnRotation(0, 0, 0);

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		FGuid fGuid;
		tGUID tGuid = npc.GetGuid();
		CopyMemory(&fGuid, &tGuid, sizeof(FGuid));
		SpawnParams.Name = FName(*fGuid.ToString());		
		
		ATNpc* SpawnEnemyCharacter = world->SpawnActor<ATNpc>(NpcCharacterClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (SpawnEnemyCharacter)
		{
			SpawnEnemyCharacter->SpawnDefaultController();
			SpawnEnemyCharacter->m_tGuid = npc.GetGuid();
			SpawnEnemyCharacter->m_fHealth = npc.GetHealth();
		}
	}
}
void AGamePlayerController::DeadEnemyCharacter(TCharacter npc)
{
	UWorld* const world = GetWorld();
	if (world)
	{
		TArray<AActor*> npcArray;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATNpc::StaticClass(), npcArray);

		for (auto Actor : npcArray)
		{
			ATNpc * tNpc = Cast<ATNpc>(Actor);
			if (tNpc && tNpc->m_tGuid == npc.GetGuid())
			{
				tNpc->Dead();
				break;
			}
		}
	}
}
void AGamePlayerController::MoveEnemyNPC(TCharacter& srcChar, TCharacter& deskChar)
{
	UWorld* const world = GetWorld();
	if (world)
	{
		TArray<AActor*> npcArray;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATNpc::StaticClass(), npcArray);
		ATNpc * tNpc = nullptr;
		for (auto Actor : npcArray)
		{
			tNpc = Cast<ATNpc>(Actor);
			if (tNpc && tNpc->m_tGuid == srcChar.GetGuid())
			{
				tNpc->MoveToLocation(deskChar.GetPos());
				break;
			}
		}
	}
}
void AGamePlayerController::RecvMoveTo(TCharacter& srcChar, FVector& vTarget )
{
	UWorld* const world = GetWorld();
	if (world)
	{
		TArray<AActor*> pcArray;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATBaseCharacter::StaticClass(), pcArray);
		ATBaseCharacter * tPC = nullptr;
		for (auto Actor : pcArray)
		{
			tPC = Cast<ATBaseCharacter>(Actor);
			if (tPC && m_tGuid != srcChar.GetGuid()
				&& tPC->m_tGuidACharacter == srcChar.GetGuid())
			{
				tPC->MoveToLocation(vTarget);
				break;
			}
		}
	}
}
void AGamePlayerController::SendMoveTo(FVector& vTarget)
{
	if (m_pGameInstance->bIsConnected == false) return;
	TCharacter tChar = GetCharacterInfo();
	tChar.SetTarget(vTarget);
	if (isDeadCharacter == false)
	{
		m_pTNet->SendMoveTo(tChar, vTarget);
	}
	//if (GEngine)GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, tChar.GetPos().ToString());
}
void AGamePlayerController::AttackEnemyCharacter(TCharacter& srcChar, TCharacter& deskChar)
{
	UWorld* const world = GetWorld();
	if (world)
	{
		TArray<AActor*> npcArray;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATNpc::StaticClass(), npcArray);

		for (auto Actor : npcArray)
		{
			ATNpc * tNpc = Cast<ATNpc>(Actor);
			if (tNpc && tNpc->m_tGuid == srcChar.GetGuid())
			{				
				tNpc->AttackMontage();
				//UE_LOG(LogClass, Log, TEXT("%s"), TEXT("pNpc->AttackMontage()"));
				break;
			}
		}
	}
}

void AGamePlayerController::TestCamera()
{
	// Fast storage for camera (for example only)
	static ACameraActor* pCamera = NULL;

	if (!pCamera)
	{
		// Get current pawn camera
		FMinimalViewInfo info;
		GetPawn()->CalcCamera(0, info);

		// Spawn camera
		FTransform spawnTM(info.Rotation, info.Location);
		//pCamera = Cast<ACameraActor>(UGameplayStatics::BeginSpawningActorFromClass(this, ACameraActor::StaticClass(), spawnTM));
		pCamera = Cast<ACameraActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ACameraActor::StaticClass(), spawnTM));
		if (pCamera)
		{
			UGameplayStatics::FinishSpawningActor(pCamera, spawnTM);
		}
	}
	else
	{
		// Teleport camera
		FViewTargetTransitionParams params;
		params.BlendTime = 1; // I want not make smooth camera changing (interpolation)
		PlayerCameraManager->SetViewTarget(pCamera, params);
	}
}