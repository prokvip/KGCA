// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SampleSocket_0Character.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Classes/Components/SphereComponent.h"
#include "Engine/World.h"
#include "GamePlayerController.h"
#include "TNpc.h"
#include "TimerManager.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
//////////////////////////////////////////////////////////////////////////
// ASampleSocket_0Character

ASampleSocket_0Character::ASampleSocket_0Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	m_fHealth = 1.0f;
	m_fEnergy = 1.0f;
	m_bHitEnable = true;

	ZeroMemory(&m_InitState, sizeof(TCharacter));
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASampleSocket_0Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveForward", this, &ASampleSocket_0Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASampleSocket_0Character::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASampleSocket_0Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASampleSocket_0Character::LookUpAtRate);
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ASampleSocket_0Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ASampleSocket_0Character::TouchStopped);
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ASampleSocket_0Character::OnResetVR);
	PlayerInputComponent->BindKey(EKeys::Escape, IE_Released, this, &ASampleSocket_0Character::EscKeyToDisConnected);
}
void ASampleSocket_0Character::EscKeyToDisConnected()
{
	UTGameInstance* m_pGameInstance = Cast<UTGameInstance>(GetGameInstance());
	if (m_pGameInstance != nullptr)
	{
		m_pGameInstance->DisConnected();
	}
}

void ASampleSocket_0Character::BeginPlay()
{
	Super::BeginPlay();
	
}
void ASampleSocket_0Character::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	//UE_LOG(LogClass, Log, TEXT("EndPlay"));
}

void ASampleSocket_0Character::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ASampleSocket_0Character::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ASampleSocket_0Character::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ASampleSocket_0Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASampleSocket_0Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
void ASampleSocket_0Character::MoveToLocation(const FVector& dest)
{
	AGamePlayerController* pController = Cast<AGamePlayerController>(GetController());
	if (pController)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(pController, dest);
	}
}
void ASampleSocket_0Character::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);

		AGamePlayerController* m_pPlayerController = Cast<AGamePlayerController>(GetWorld()->GetFirstPlayerController());
		FVector vTarget = GetActorLocation() + Direction*Value * 10000.0f;
		m_pPlayerController->SendMoveTo(vTarget);
	}
}

void ASampleSocket_0Character::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);


		AGamePlayerController* m_pPlayerController = Cast<AGamePlayerController>(GetWorld()->GetFirstPlayerController());
		FVector vTarget = GetActorLocation() + Direction * Value * 10000.0f;
		m_pPlayerController->SendMoveTo(vTarget);
	}
}

void ASampleSocket_0Character::AttackOtherCharacter()
{
	//UE_LOG(LogClass, Log, TEXT("Hit!"));
	if (m_bHitEnable)
	{
		m_bHitEnable = false;
		GetWorldTimerManager().SetTimer(HitTimerHandle, this, &ASampleSocket_0Character::ResetHitEnable, 0.5f, false, 0.5f);
	}
	else
	{
		return;
	}

	// 공격 반경내의 캐릭터리스트
	TArray<UActorComponent*> Comps;
	TArray<AActor*> NearCharacters;
	this->GetComponents(Comps);
	for (auto Comp : Comps)
	{
		USphereComponent* HitSphere = Cast<USphereComponent>(Comp);
		if (HitSphere)
		{
			HitSphere->GetOverlappingActors(NearCharacters);
			break;
		}
	}

	for (auto Character : NearCharacters)
	{
		// player
		ASampleSocket_0Character * OtherCharacter = Cast<ASampleSocket_0Character>(Character);
		if (OtherCharacter)
		{
			if ( OtherCharacter->m_tGuidACharacter != m_tGuidACharacter)
			{
				AGamePlayerController* m_pPlayerController = Cast<AGamePlayerController>(GetWorld()->GetFirstPlayerController());
				FGuid fGuid;
				CopyMemory(&fGuid, &OtherCharacter->m_tGuidACharacter, sizeof(FGuid));
				m_pPlayerController->HitCharacter(fGuid);
			}
		}
		// npc
		else
		{
			ATNpc * tNpc = Cast<ATNpc>(Character);
			if (tNpc)
			{
				AGamePlayerController* m_pPlayerController = Cast<AGamePlayerController>(GetWorld()->GetFirstPlayerController());
				m_pPlayerController->EnemyHit(tNpc->m_tGuid);
				tNpc->HitReact();
			}
		}
	}
}
void ASampleSocket_0Character::ResetHitEnable()
{
	m_bHitEnable = true;
}

void ASampleSocket_0Character::Jump()
{
	ACharacter::Jump();
}
void ASampleSocket_0Character::PossessedBy(AController* NewController )
{
	Super::PossessedBy(NewController);
	check(NewController);
}
void ASampleSocket_0Character::UpdateHealth(float HealthChange)
{
	m_fHealth += HealthChange;
}

float ASampleSocket_0Character::GetHealth()
{
	return m_fHealth;
}

bool ASampleSocket_0Character::IsFalling()
{
	return GetCharacterMovement()->IsFalling();
}
