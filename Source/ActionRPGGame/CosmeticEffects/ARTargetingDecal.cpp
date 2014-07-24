// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "ParticleHelper.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

#include "../BlueprintLibrary/ARTraceStatics.h"

#include "../ARCharacter.h"

#include "ARTargetingDecal.h"

UARTargetingDecal::UARTargetingDecal(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	//it should tick because we need to update position, when player move.
	//we also don't really care at which thread it will tick, since it's only cosmetics.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bRunOnAnyThread = true;
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_DuringPhysics;
	bTickInEditor = true;
	bAllowConcurrentTick = true;
	bAutoRegister = true;
	UpdateInterval = 0.1f;
	DecalSize = FVector(200, 200, 100);
	
}

void UARTargetingDecal::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentUpdateInterval += DeltaTime;
	if (CurrentUpdateInterval >= UpdateInterval)
	{
		if (DecalComponent)
		{
			UpdateDecalComponentPosition();
		}
	}
}

void UARTargetingDecal::DestroyComponent()
{
	if (DecalComponent)
		DecalComponent->DestroyComponent();
	Super::DestroyComponent();
}

void UARTargetingDecal::Initialize(APawn* OwningCharacterIn)
{
	Activate();
	OwningCharacter = OwningCharacterIn;
	FHitResult HitLocation = UARTraceStatics::GetHitResult(10000, NAME_None, OwningCharacterIn, false, false, EARTraceType::Trace_Weapon);
	DecalComponent = UGameplayStatics::SpawnDecalAtLocation(OwningCharacterIn, DecalMaterial, DecalSize, HitLocation.Location, FRotator(-90, 0, 0));
}

void UARTargetingDecal::UpdateDecalComponentPosition()
{
	FHitResult HitLocation = UARTraceStatics::GetHitResult(10000, NAME_None, OwningCharacter, false, false, EARTraceType::Trace_Weapon);

	DecalComponent->SetWorldLocation(HitLocation.Location);
}