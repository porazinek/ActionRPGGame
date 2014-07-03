// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"
#include "Types/ARStructTypes.h"
#include "GameplayTagsModule.h"

#include "ARProjectile.h"

AARProjectile::AARProjectile(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	Movement = PCIP.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("Movement"));

	SetReplicates(true);
	bReplicateInstigator = true;
	bReplicateMovement = true;
}
