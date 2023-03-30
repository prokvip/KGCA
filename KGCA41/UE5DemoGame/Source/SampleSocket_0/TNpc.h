// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TGameInstance.h"
#include "TNpc.generated.h"

UCLASS()
class SAMPLESOCKET_0_API ATNpc : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATNpc();
public:
	tGUID	m_tGuid;		// 플레이어 고유 아이디
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	// 체력(EditAnywhere:blueprint 편집, VisibleAnywhere:보이기만 한다.)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KGCA", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "100.0"))
		float m_fHealth;
	// 체력을 업데이트한다.
	UFUNCTION(BlueprintCallable, Category = "KGCA")
		void UpdateHealth(float HealthChange);
	// 체력을 얻는다.
	UFUNCTION(BlueprintPure, Category = "KGCA")
		float GetHealth();
public:
	// Movement
	void MoveToLocation(const FVector& dest);
	// Attack
	UFUNCTION(BlueprintImplementableEvent)
		void AttackMontage();
	// Dead
	UFUNCTION(BlueprintImplementableEvent)
		void Dead();
	// 공격 반응
	UFUNCTION(BlueprintImplementableEvent)
		void HitReact();
};
