// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TNetworkManager.h"
#include "TGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TBASISCORE_API UTGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:		
	UTGameInstance(const class FObjectInitializer& PCIP);
protected:
	UPROPERTY(Transient)
	UTNetworkManager* m_pTNet;
public:
	UFUNCTION(BlueprintPure,Category = "KGCA",meta = (DisplayName = "UTNetworkManager",	Keywords = "UTNetworkManager"))
		UTNetworkManager *GetNetworkMgr();

	virtual void StartGameInstance() override;
	virtual void OnStart() override;
	virtual void Init() override;
	virtual void Shutdown() override;	
public:
	UFUNCTION()
		bool Connected(const FString& ip, int iPort);
	UFUNCTION()
		bool DisConnected();
	UPROPERTY()
		bool bIsConnected;

	int m_iServerState;
};
