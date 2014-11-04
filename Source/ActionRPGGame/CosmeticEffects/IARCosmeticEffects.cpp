// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"
#include "IARCosmeticEffects.h"

UIARCosmeticEffects::UIARCosmeticEffects(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

FVector IIARCosmeticEffects::GetOriginLocation()
{
	return FVector::ZeroVector;
}