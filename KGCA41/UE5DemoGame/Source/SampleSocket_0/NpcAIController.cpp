// Fill out your copyright notice in the Description page of Project Settings.

#include "NpcAIController.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "DrawDebugHelpers.h"

ANpcAIController::ANpcAIController()
{
	RepeatInterval = 3.0f;
}
void ANpcAIController::BeginPlay()
{
	Super::BeginPlay();
	//GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &ANpcAIController::OnRepeatTimer, RepeatInterval, true);
}
void ANpcAIController::MoveTarget(const FVector& desk)
{
	if (GetPawn() == nullptr) return;
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, desk);
	//GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Yellow, "MoveTarget");
}

void ANpcAIController::GetNavigationPath(const FVector& desk)
{
	UNavigationSystemV1* navSys = UNavigationSystemV1::GetCurrent(GetWorld());
	UNavigationPath* path = navSys->FindPathToLocationSynchronously(
		GetWorld(), GetPawn()->GetActorLocation(), desk, NULL);
	if (path != NULL)
	{
		for (int pointiter = 0; pointiter < path->PathPoints.Num(); pointiter++)
		{
			DrawDebugSphere(GetWorld(), path->PathPoints[pointiter], 10.0f, 12, FColor(255, 0, 0));
		}
	}
}
void ANpcAIController::OnRepeatTimer()
{
	if (GetPawn() == nullptr) return;
	auto CurrentPawn = GetPawn();
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation NextLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextLocation))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation);
	}
}
