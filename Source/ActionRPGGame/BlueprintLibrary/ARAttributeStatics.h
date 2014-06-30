// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "../Types/ARStructTypes.h"
#include "../Types/AREnumTypes.h"
#include "ARAttributeStatics.generated.h"

/*
	Helper function for attributes.
	Mainly meant for things like damage formulas, (armor reduction, damage increase), which are based on 
	Attributes of actor.
	Inteded use of these function is to place them as last node before applying attribute change (damage
	or healing).

	In the essence these replace, what you would do in TakeDamage, and move it to damage causer,
	which will calculate damage.
*/
UCLASS()
class UARAttributeStatics : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "AR|RPG Math")
		static float ReduceDamageByArmor(AActor* DamageTarget, AActor* Instigator, float DamageIn);
};



