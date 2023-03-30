// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NpcAIController.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLESOCKET_0_API ANpcAIController : public AAIController
{
	GENERATED_BODY()
public:
	ANpcAIController();
	virtual void BeginPlay() override;
	void MoveTarget(const FVector& desk);
	void GetNavigationPath(const FVector& desk);
private:
	void OnRepeatTimer();
	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;
};
