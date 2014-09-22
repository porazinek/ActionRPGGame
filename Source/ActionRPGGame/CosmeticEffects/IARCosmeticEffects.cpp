// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"
#include "IARCosmeticEffects.h"

UIARCosmeticEffects::UIARCosmeticEffects(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{

}

FVector IIARCosmeticEffects::GetOriginLocation()
{
	return FVector::ZeroVector;
}