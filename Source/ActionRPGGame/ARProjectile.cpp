// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"
#include "Types/ARStructTypes.h"
#include "GameplayTagsModule.h"

#include "ParticleHelper.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

#include "ARProjectile.h"

AARProjectile::AARProjectile(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	Collision = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("Collision"));
	Collision->InitSphereRadius(10.0f);
	Collision->bTraceComplexOnMove = true;
	Collision->IgnoreActorWhenMoving(GetOwner(), true);
	RootComponent = Collision;
	Movement = PCIP.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("Movement"));
	Movement->UpdatedComponent = Collision;
	Movement->bRotationFollowsVelocity = true;
	Movement->OnProjectileStop.AddDynamic(this, &AARProjectile::OnStop);

	SetReplicates(true);
	bReplicateInstigator = true;
	bReplicateMovement = true;
}

void AARProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	for (AActor* actor : ActorToIgnore)
	{
		Collision->IgnoreActorWhenMoving(actor, true);
	}
	ActorToIgnore.Empty();
}

void AARProjectile::OnStop(const FHitResult& Hit)
{
	OnProjectileHit(Hit.Actor.Get());
}

void AARProjectile::OnProjectileHit_Implementation(AActor* OtherActor)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnHitEffect, GetActorLocation(), FRotator(0, 0, 0), true);
	Movement->SetActive(false);
	bHidden = true;
	SetLifeSpan(2.0f);
}