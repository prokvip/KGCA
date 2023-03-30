// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TProjectile.generated.h"

UCLASS()
class SAMPLESOCKET_0_API ATProjectile : public AActor
{
	GENERATED_BODY()
private:
	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = "KGCA")
		class USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KGCA", meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;
public:
	ATProjectile();

	UPROPERTY(VisibleAnywhere)
		class UParticleSystemComponent* PSC;

	/** called when projectile hits something */
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

};
