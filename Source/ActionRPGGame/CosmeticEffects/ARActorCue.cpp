// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "ParticleHelper.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

#include "../ARCharacter.h"

#include "ARActorCue.h"

AARActorCue::AARActorCue(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	//AttachRootComponentToActor(AttachTo.Get(), AttachName, EAttachLocation::SnapToTarget);
	ArrowRoot = PCIP.CreateDefaultSubobject<UArrowComponent>(this, TEXT("ArrowRoot"));
	SetRootComponent(ArrowRoot);
}

void AARActorCue::InitializeAttachment(AActor* AttachToIn, FName AttachNameIn)
{
	if (!AttachToIn && AttachName == NAME_None && !ParticleSystem.IsValid())
		return;
	AARCharacter* MyChar = Cast<AARCharacter>(AttachToIn);
	if (!MyChar)
		return;
	//AttachRootComponentToActor(AttachToIn, AttachNameIn, EAttachLocation::KeepWorldPosition);
	
	ParticleSystemComp = UGameplayStatics::SpawnEmitterAttached(ParticleSystem.Get(), this->ArrowRoot, NAME_None, FVector(0, 0, 0), FRotator(0, 0, 0), EAttachLocation::Type::SnapToTarget);
}