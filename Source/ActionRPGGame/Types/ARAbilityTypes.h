// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "AREnumTypes.h"
#include "ARAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FARAbilityUpgrades
{
	GENERATED_USTRUCT_BODY()
public:
	//this will indicate if we should spawn upgrade and add it to ability, 
	//while it become active (spawned)
	//No need to spawn all objects just to check if they should be spawned in first place ;)
	UPROPERTY(EditAnywhere, Category = "Upgrade Active")
		bool IsUpgradeActive;

	//add TSubclassOf Of Upgrade. It probabaly won't be subclass of Effect, even if, it's going
	//to have very specialized code and will work only with Ability.
	//generic systems are evil!
};
