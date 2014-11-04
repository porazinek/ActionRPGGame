// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../Types/AREnumTypes.h"

#include "../BlueprintLibrary/ARTraceStatics.h"
#include "../ARCharacter.h"
#include "ParticleHelper.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

#include "Net/UnrealNetwork.h"

#include "AREffectCue.h"

UAREffectCue::UAREffectCue(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicated(true);
}
