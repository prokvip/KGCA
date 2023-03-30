// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TGameMode.h"
#include "LoginGameMode.generated.h"

class AGamePlayerController;
/**
 * 
 */
UCLASS()
class SAMPLESOCKET_0_API ALoginGameMode : public ATGameMode
{
	GENERATED_BODY()	
public:
	class AGamePlayerController*  m_pPlayerController;
public:
	virtual void BeginPlay() override;
	virtual void ProcessPacket();
public:
	ALoginGameMode();
};
