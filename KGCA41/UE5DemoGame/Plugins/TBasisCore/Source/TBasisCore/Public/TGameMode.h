// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TGameInstance.h"
#include "TNetworkManager.h"
#include "TGameMode.generated.h"

// 반드시 F로 델리게이트 이름은 시작해야 한다.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLoginAckOKDelegate, int, retValue);

/**
 * 
 */
UCLASS()
class TBASISCORE_API ATGameMode : public AGameModeBase
{
	GENERATED_BODY()
private:
	typedef void(ATGameMode::*CallFuction)(UPACKET& t);
	std::map<int, CallFuction>		m_fnExecutePacket;
public:
	DECLARE_EVENT_OneParam(ALoginGameMode, Event_AccountAck, int);
	Event_AccountAck& OnAccountAck(int iValue) { return AccountAck; }
	Event_AccountAck AccountAck;
	UPROPERTY()
		UTGameInstance*  m_pGameInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "KGCA")
		bool				IsVerification;

	FTimerHandle		TimerHandle;
	// 사망시 임시 카메라 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KGCA")
		ACameraActor* pDeadCamera;
	// 스폰되는 캐릭터 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KGCA")
		FTransform  InitSpawnTransform;
	// 사망시 캐릭터 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KGCA")
		FTransform  CameraAtDeathTransform;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "KGCA", Meta = (BlueprintProtect = "true"))
		TSubclassOf<class UUserWidget> DefaultWidgetClass;
	UPROPERTY()
		class UUserWidget* DefalultWidget;
public:
	UFUNCTION()
		void LoginOK(int retValue);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "KGCA")
		FLoginAckOKDelegate OnLoginOKDelegate;
	DECLARE_DELEGATE_RetVal_OneParam(int, FLoginAckDelegate, int);
	FLoginAckDelegate OnLoginAckDelegate;	
	
	UFUNCTION(BlueprintCallable, Category = "KGCA")
		bool Connected(const FString& ip, int iPort);
	
	UFUNCTION(BlueprintCallable, Category = "KGCA")
		bool DisConnected();

	UFUNCTION(BlueprintCallable, Category = "KGCA")
		bool LoginServer(const FText& Id, const FText& Pw);

	UFUNCTION(BlueprintCallable, Category = "KGCA")
		bool CreateAccount(const FText& Id, const FText& Pw);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "KGCA")
		int LoginAck(int iReturnValue);
	virtual int LoginAck_Implementation(int iReturnValue);

	UFUNCTION(BlueprintNativeEvent, Category = "KGCA")
		int CreateAccountAck(int iReturnValue);
	virtual int CreateAccountAck_Implementation(int iReturnValue);

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;
protected:
	UTNetworkManager * m_pTNet;
	virtual void ProcessPacket();

public:
	ATGameMode();
};
