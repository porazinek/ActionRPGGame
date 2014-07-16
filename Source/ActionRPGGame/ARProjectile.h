// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "ARProjectile.generated.h"

UCLASS()
class AARProjectile : public AActor
{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
		TSubobjectPtr<class USphereComponent> Collision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		TSubobjectPtr<class UProjectileMovementComponent> Movement;

	UPROPERTY(BlueprintReadWrite, Category = "Ownership")
		AController* InstigatorController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmetic Effects")
		UParticleSystem* OnHitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Tags")
		FGameplayTagContainer OwnedTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Tags")
		FGameplayTagContainer RequiredTags;

	UFUNCTION()
		virtual void OnStop(const FHitResult& Hit);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Collision")
		void OnProjectileHit(AActor* OtherActor);
	
	/*
		Something I picked up in UT code, to fix projectiles no appear to clients, while
		networked play.
	*/
	FActorTickFunction InitialReplicationTick;

};



