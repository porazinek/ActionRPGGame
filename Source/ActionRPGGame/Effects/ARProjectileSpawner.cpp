// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../BlueprintLibrary/AREffectStatics.h"

#include "ARProjectileSpawner.h"

AARProjectileSpawner::AARProjectileSpawner(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetReplicates(true);
}

void AARProjectileSpawner::SpawnProjectiles()
{
	FTimerManager& IntervalTimer = GetWorld()->GetTimerManager();
	FTimerManager& DurationTimer = GetWorld()->GetTimerManager();

	
	FTimerDelegate IntervalDelegate = FTimerDelegate::CreateUObject(this, &AARProjectileSpawner::SpawnProjectileInterval);
	IntervalTimer.SetTimer(IntervalHandle, IntervalDelegate, ProjectileInfo.Interval, true, 0);
	
	FTimerDelegate DurationDelegate = FTimerDelegate::CreateUObject(this, &AARProjectileSpawner::StopSpawningProjectiles);
	DurationTimer.SetTimer(DurationHandle, DurationDelegate, AmountOfBursts*ProjectileInfo.Interval+KINDA_SMALL_NUMBER, false);
}

void AARProjectileSpawner::SpawnProjectileInterval()
{
	for (int32 CurAmount = 0; CurAmount < ProjectileInfo.BurstSize; CurAmount++)
	{
		FVector Location = ProjectileInfo.Location;
		Location.Z += ProjectileInfo.MaxHeight + FMath::FRandRange(-ProjectileInfo.MaxHeightOffset, ProjectileInfo.MaxHeightOffset);
		Location.Y += FMath::RandRange(-ProjectileInfo.MaxRadius, ProjectileInfo.MaxRadius);
		Location.X += FMath::RandRange(-ProjectileInfo.MaxRadius, ProjectileInfo.MaxRadius);
		FTransform SpawnTM(FRotator(0, 0, 0), Location);

		AARProjectile* proj = Cast<AARProjectile>(UGameplayStatics::BeginSpawningActorFromClass(this, ProjectileClass, SpawnTM));

		FVector FallDirection = FVector(FMath::FRandRange(-ProjectileInfo.ImpactDirection, ProjectileInfo.ImpactDirection), FMath::FRandRange(-ProjectileInfo.ImpactDirection, ProjectileInfo.ImpactDirection), -1);

		if (proj)
		{
			//proj->Instigator = Causer;
			proj->SetOwner(GetOwner());
			proj->Instigator = Instigator;
			proj->Movement->Velocity = FallDirection * ProjectileInfo.InitialVelocity; // proj->Movement->InitialSpeed;
			UGameplayStatics::FinishSpawningActor(proj, SpawnTM);
		}
	}
}

void AARProjectileSpawner::StopSpawningProjectiles()
{
	FTimerManager& TimerToClear = GetWorld()->GetTimerManager();
	TimerToClear.ClearTimer(IntervalHandle);

	//in case we need to do some cleanup.
	this->SetLifeSpan(1);
}