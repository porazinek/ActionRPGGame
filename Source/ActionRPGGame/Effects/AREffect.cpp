// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"
#include "AREffect.h"

UAREffect::UAREffect(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}

void UAREffect::InitializeEffect()
{
	OnEffectInitialized();
//	OnEffectInit.Broadcast();
}

UWorld* UAREffect::GetWorld() const
{
	if (EffectTarget.IsValid())
	{
		UWorld* const world = GEngine->GetWorldFromContextObject(EffectTarget.Get());
		return world ? world : NULL;
	}
	return NULL;
}

