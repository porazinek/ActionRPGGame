// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "../Types/ARStructTypes.h"

#include "ARItem.generated.h"

/*
	Item is base class for objects that:
	
	1. Base class for items which can be attached to pawns. Like Weapons.
*/
UCLASS()
class AARItem : public AActor
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, Category = "Attributes")
		TArray<FAttribute> ItemAttributes;

	UPROPERTY()
	class AARCharacter* ItemOwner;
};



