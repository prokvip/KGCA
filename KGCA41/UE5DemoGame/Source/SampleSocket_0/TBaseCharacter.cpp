// Fill out your copyright notice in the Description page of Project Settings.

#include "TBaseCharacter.h"
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
// ATBaseCharacter

ATBaseCharacter::ATBaseCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;


	tCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("ttSpringArm"));
	tCameraBoom->SetupAttachment(RootComponent);
	tCameraBoom->TargetArmLength = 300.0f;
	tCameraBoom->bUsePawnControlRotation = true;

	tFollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ttFollowCamera"));
	tFollowCamera->SetupAttachment(tCameraBoom, USpringArmComponent::SocketName);
	tFollowCamera->bUsePawnControlRotation = false;

	m_fHealth = 1.0f;
	m_fEnergy = 1.0f;
	m_bHitEnable = true;

	ZeroMemory(&m_InitState, sizeof(TCharacter));
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATBaseCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveForward", this, &ATBaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATBaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ATBaseCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATBaseCharacter::LookUpAtRate);
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ATBaseCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ATBaseCharacter::TouchStopped);
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ATBaseCharacter::OnResetVR);
	PlayerInputComponent->BindKey(EKeys::Escape, IE_Released, this, &ATBaseCharacter::EscKeyToDisConnected);
}
void ATBaseCharacter::EscKeyToDisConnected()
{
	UTGameInstance* m_pGameInstance = Cast<UTGameInstance>(GetGameInstance());
	if (m_pGameInstance != nullptr)
	{
		m_pGameInstance->DisConnected();
	}
}

void ATBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

}
void ATBaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	//UE_LOG(LogClass, Log, TEXT("EndPlay"));
}

void ATBaseCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ATBaseCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ATBaseCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ATBaseCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATBaseCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
void ATBaseCharacter::MoveToLocation(const FVector& dest)
{
	AGamePlayerController* pController = Cast<AGamePlayerController>(GetController());
	if (pController)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(pController, dest);
	}
}
void ATBaseCharacter::MoveForward(float Value)
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
		FVector vTarget = GetActorLocation() + Direction * Value * 10000.0f;
		m_pPlayerController->SendMoveTo(vTarget);
	}
}

void ATBaseCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
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

void ATBaseCharacter::AttackOtherCharacter()
{
	//UE_LOG(LogClass, Log, TEXT("Hit!"));
	if (m_bHitEnable)
	{
		m_bHitEnable = false;
		GetWorldTimerManager().SetTimer(HitTimerHandle, this, &ATBaseCharacter::ResetHitEnable, 0.5f, false, 0.5f);
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
		ATBaseCharacter* OtherCharacter = Cast<ATBaseCharacter>(Character);
		if (OtherCharacter)
		{
			if (OtherCharacter->m_tGuidACharacter != m_tGuidACharacter)
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
			ATNpc* tNpc = Cast<ATNpc>(Character);
			if (tNpc)
			{
				AGamePlayerController* m_pPlayerController = Cast<AGamePlayerController>(GetWorld()->GetFirstPlayerController());
				m_pPlayerController->EnemyHit(tNpc->m_tGuid);
				tNpc->HitReact();
			}
		}
	}
}
void ATBaseCharacter::ResetHitEnable()
{
	m_bHitEnable = true;
}

void ATBaseCharacter::Jump()
{
	ACharacter::Jump();
}
void ATBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	check(NewController);
}
void ATBaseCharacter::UpdateHealth(float HealthChange)
{
	m_fHealth += HealthChange;
}

float ATBaseCharacter::GetHealth()
{
	return m_fHealth;
}

bool ATBaseCharacter::IsFalling()
{
	return GetCharacterMovement()->IsFalling();
}


