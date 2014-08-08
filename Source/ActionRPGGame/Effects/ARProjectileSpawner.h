// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "../Types/ARStructTypes.h"

#include "ARProjectileSpawner.generated.h"

/*
	Spawn projectiles over time in set area.
*/
UCLASS()
class AARProjectileSpawner : public AActor
{
	GENERATED_UCLASS_BODY()
public:

	void SpawnProjectiles();

	UFUNCTION()
		void SpawnProjectileInterval();

	UFUNCTION()
		void StopSpawningProjectiles();

	/*
		Projectile to spawn
	*/
	UPROPERTY(EditAnywhere, Category = "Properties")
		TSubclassOf<class AARProjectile> ProjectileClass; //make it array ?

	UPROPERTY()
		FARProjectileInfo ProjectileInfo;

	int32 AmountOfBursts;

	FTimerHandle IntervalHandle;

	FTimerHandle DurationHandle;
};



