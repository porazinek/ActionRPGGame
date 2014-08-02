// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"
#include "../../ARPlayerController.h"
#include "../../Abilities/ARAbility.h"
#include "ARAbilityCostEffect.h"

UARAbilityCostEffect::UARAbilityCostEffect(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}

void UARAbilityCostEffect::Initialize()
{
	Super::Initialize();

	if (EffectTarget.IsValid())
	{
		AARPlayerController* MyPC = Cast<AARPlayerController>(EffectTarget.Get());
		MyPC->OnAbilityEquiped.AddDynamic(this, &UARAbilityCostEffect::ChangeAbilityCost);
	}
}

void UARAbilityCostEffect::ChangeAbilityCost(class AARAbility* AbilityIn)
{
	if (AbilityIn)
	{
		for (FAttribute& attr : AbilityIn->ResourceCost)
		{
			attr.ModValue -= 10;
		}
	}
}