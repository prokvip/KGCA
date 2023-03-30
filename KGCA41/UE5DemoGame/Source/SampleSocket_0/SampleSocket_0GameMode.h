// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "TGameMode.h"
#include "SampleSocket_0GameMode.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerDelegate, TCharacter*, pCharacter);

UCLASS()
class ASampleSocket_0GameMode : public ATGameMode
{
	GENERATED_BODY()
public:
	//UPROPERTY(BlueprintAssignable, Category = "KGCA")
	//	FPlayerDelegate RecvZoneNewCharacter;
	class AGamePlayerController*  m_pPlayerController;
public:
	virtual void BeginPlay() override;
	virtual void ProcessPacket();
public:
	UFUNCTION()
		void OnCharacterReset();
	UFUNCTION()
		void OnCharacterRespawn(AActor* DestroyedActor);

	UFUNCTION(BlueprintCallable, Category = "KGCA")
		void OnUserCharacterKill();

	void SendTargetPointInfo();
public:
	ASampleSocket_0GameMode();
};



