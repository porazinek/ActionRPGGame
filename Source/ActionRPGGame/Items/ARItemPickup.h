// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "../Types/ARStructTypes.h"
#include "../Types/ARItemTypes.h"
#include "ARItemPickup.generated.h"

/*
	
*/
UCLASS()
class AARItemPickup : public AActor
{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Pickup Item")
		FARItemPickerContainer ItemContainer;

	//It will attemp to add ALL items from pick to player inventory.
	//For testing now, might use it later.
	void GiveAllItemsTo(class AARPlayerController* PickupInstigator);
};



