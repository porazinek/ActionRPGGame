// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "Net/UnrealNetwork.h"
#include "../ARCharacter.h"
#include "../ActionState/ARActionStateComponent.h"

#include "ParticleHelper.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

#include "ARMeleWeapon.h"

AARMeleWeapon::AARMeleWeapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bNetUseOwnerRelevancy = true;
	bReplicateInstigator = true;
	bReplicates = true;
}
