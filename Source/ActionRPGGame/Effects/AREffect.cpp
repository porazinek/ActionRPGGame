// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"
#include "AREffect.h"

UAREffect::UAREffect(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UAREffect::InitializeEffect()
{
	OnEffectInitialized(EffectInstigator.Get());
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

