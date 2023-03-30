// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TGameInstance.h"
#include "TBaseCharacter.generated.h"

UCLASS(config = Game)
class SAMPLESOCKET_0_API ATBaseCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* tCameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* tFollowCamera;

	ATBaseCharacter();
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PossessedBy(AController* NewController) override;
protected:
	void EscKeyToDisConnected();
	void OnResetVR();
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return tCameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return tFollowCamera; }
public:
	// Health(EditAnywhere:blueprint ����, VisibleAnywhere:���̱⸸ �Ѵ�.)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KGCA", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "100.0"))
		float m_fHealth;
	// Energy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KGCA", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float m_fEnergy;
	// Health
	UFUNCTION(BlueprintCallable, Category = "KGCA")
		void UpdateHealth(float HealthChange);
	// Health
	UFUNCTION(BlueprintPure, Category = "KGCA")
		float GetHealth();
	// BlueprintNativeEvent = BlueprintImplementableEvent + BlueprintCallable
	//                        C++������ �Լ��̸� �ڿ� _Implementation�� ������ �����Ѵ�.
	//                        �������Ʈ ������ ��� C++ ���� �Լ��� ȣ��ȴ�.

	// Damage: �������Ʈ������ �ۼ� ������. : 
	UFUNCTION(BlueprintImplementableEvent)
		void DamageAnimation();
	// Damage: �������Ʈ������ �ۼ� ������. : 
	UFUNCTION(BlueprintImplementableEvent)
		void DeadAnimation();
	// Play Character Attack : �������Ʈ������ �ۼ� ������. : 
	UFUNCTION(BlueprintImplementableEvent)
		void AttackMontage();
	// User Attack : BlueprintCallable : �������Ʈ���� ȣ�⸸ ���� �����Ұ�
	UFUNCTION(BlueprintCallable)
		void AttackOtherCharacter();
	bool IsFalling();

	void MoveToLocation(const FVector& dest);
public:
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Properties")
	tGUID		m_tGuidACharacter;
	TCharacter  m_InitState;
protected:
	FTimerHandle HitTimerHandle;
	bool m_bHitEnable;
	void ResetHitEnable();
private:
	// Jump
	void Jump();

};
