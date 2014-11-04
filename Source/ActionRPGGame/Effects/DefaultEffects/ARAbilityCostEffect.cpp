// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"
#include "../../ARPlayerController.h"
#include "../../Abilities/ARAbility.h"
#include "../../Abilities/ARAbilityComponent.h"

#include "ARAbilityCostEffect.h"

UARAbilityCostEffect::UARAbilityCostEffect(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UARAbilityCostEffect::Initialize()
{
	Super::Initialize();

	if (EffectTarget.IsValid())
	{
		UARAbilityComponent* tempComp = EffectTarget->FindComponentByClass<UARAbilityComponent>();
		if (tempComp)
		{
			//tempComp->OnActionAddedToBar.BindUObject(this, &UARAbilityCostEffect::ModifyAbility);
			tempComp->OnAbilityAdded.AddDynamic(this, &UARAbilityCostEffect::ModifyAbility);
		}
		//AARPlayerController* MyPC = Cast<AARPlayerController>(EffectTarget.Get());
		//MyPC->OnAbilityEquiped.AddDynamic(this, &UARAbilityCostEffect::ChangeAbilityCost);
	}
}

void UARAbilityCostEffect::ModifyAbility_Implementation(AARAbility* AbilityIn)
{
	if (AbilityIn)
	{
		for (FAttribute& attr : AbilityIn->ResourceCost)
		{
			attr.ModValue -= 10;
		}
	}
}