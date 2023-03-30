// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "SampleSocket_0Character.h"
#include "TGameInstance.h"
#include "GamePlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterDeadDelegate);

UCLASS()
class SAMPLESOCKET_0_API AGamePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	// 채팅 함수
	UFUNCTION(BlueprintCallable, Category = "KGCA")
		void ChatToServer(FString Text);
	// 세션 아이디 반환
	UFUNCTION(BlueprintPure, Category = "KGCA")
		FGuid GetSessionId();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "KGCA", Meta = (BlueprintProtect = "true"))
		TSubclassOf<class UUserWidget> GameOverWidgetClass;
	// HUD 객체
	UPROPERTY()
		class UUserWidget* DefalultWidget;
	UPROPERTY()
		class UUserWidget* GameOverWidget;
	// 스폰되는 캐릭터
	UPROPERTY(EditAnywhere, Category = "KGCA")
		TSubclassOf<class ACharacter> UserCharacterClass;
	UPROPERTY(EditAnywhere, Category = "KGCA")
		TSubclassOf<class ACharacter> NpcCharacterClass;
	// 소멸 파티클
	UPROPERTY(EditAnywhere, Category = "KGCA")
		UParticleSystem* DestroyEmiiter;
	// 공격 파티클
	UPROPERTY(EditAnywhere, Category = "KGCA")
		UParticleSystem* HitEmiiter;
	// 다른 캐릭터 타격
	UFUNCTION(BlueprintCallable, Category = "KGCA")
		void HitCharacter(const FGuid& SessionId);

	UPROPERTY(BlueprintAssignable, Category = "KGCA")
		FCharacterDeadDelegate OnUserDeadDelegate;
	UFUNCTION(BlueprintCallable, Category = "KGCA")
		void OnUserCharacterKill();
public:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnConstruction(const FTransform &Transform) override;
	virtual void OnPossess(APawn* aPawn) override; // Possess는 4.22에서 데프리케이트 됨.
	virtual void OnUnPossess() override; 
public:	
	void SendZoneEntry();
	void SpawnPCList();
	void SpawnNPCList();
	void EnemyHit(tGUID & tGuid);
	void RecvZoneEntry();
	void RecvMsg(const string msg);
	void EscKeyToDisConnected();	
	void RecvPlayerLogout(tGUID tGuid);
	void ShowFrameRate(); 
	void SendPlayerInfo();
	void SendEnemyInfo();
	bool UpdateWorldInfo();
	void UpdatePlayerInfo(const TCharacter& info);		
	void RecvZoneNewCharacter(TCharacter pCharacter);
	void UpdateEnemyList(TCharacterMap* pNpcMap);
	void RespawnCharacter(TCharacter pc);
	void SpawnEnemyCharacter(TCharacter pNpc);
	void DeadEnemyCharacter(TCharacter pNpc);
	void MoveEnemyNPC(TCharacter& srcChar, TCharacter& deskChar);
	void RecvMoveTo(TCharacter& srcChar, FVector& vTarget);
	void SendMoveTo(FVector& vTarget);
	void AttackEnemyCharacter(TCharacter& srcChar, TCharacter& deskChar);
	void DeadCharacter(TCharacter tChar);
	void DamageCharacter(TCharacter tChar);
	void DestroyCharacter();
	// 케릭터 정보 반환
	TCharacter GetCharacterInfo();	
	// tGUID 엑터반환
	AActor* FindActorBySessionId(TArray<AActor*> ActorArray, const tGUID& SessionId);
private:
	UTGameInstance*    m_pGameInstance;
	UTNetworkManager*	m_pTNet;			
	tGUID				m_tGuid;				
	ASampleSocket_0Character* m_pCharacter;		
	FTimerHandle		SendPlayerInfoHandle;	
	int					m_iNumPlayers;
	int					m_iNumEnemy;	
	int					FPS = 0;
	bool			    isDeadCharacter;
	bool				m_bGameStart;
public:
	ASampleSocket_0Character* GetCharacter();
public:
	AGamePlayerController();
	virtual ~AGamePlayerController();	
public:
	void TestCamera();
};
