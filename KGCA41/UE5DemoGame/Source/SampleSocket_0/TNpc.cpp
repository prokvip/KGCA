// Fill out your copyright notice in the Description page of Project Settings.

#include "TNpc.h"
#include "NpcAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
// Sets default values
ATNpc::ATNpc()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATNpc::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ATNpc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATNpc::MoveToLocation(const FVector& dest)
{
	ANpcAIController * Controller = Cast<ANpcAIController>(GetController());
	if (Controller)
	{
		/*//UE_LOG(LogTemp, Warning, TEXT("MOVING TOWARDS CHARACTER %s AT POSITION %s"), 
			TEXT("TEST"),	*dest.ToString());*/
		Controller->MoveTarget(dest);		
	}
}

void ATNpc::UpdateHealth(float HealthChange)
{
	m_fHealth += HealthChange;
}

float ATNpc::GetHealth()
{
	return m_fHealth;
}


