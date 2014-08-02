// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "../AREffectType.h"
#include "ARAbilityCostEffect.generated.h"

/*
	Effect for calculating cost of used ability, before it's being exectued.
	Also example of how you might implement effect in C++,
	and then add them by default to your attribute component.
*/

UCLASS(Blueprintable, BlueprintType)
class UARAbilityCostEffect : public UAREffectType
{
	GENERATED_UCLASS_BODY()
public:
	virtual void Initialize() override;

	UFUNCTION()
	void ChangeAbilityCost(class AARAbility* AbilityIn);
};



