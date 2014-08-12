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

	virtual void PostInitializeComponents() override;
	UPROPERTY(EditAnywhere, Category = "Pickup Item")
		FARItemPickerContainer ItemContainer;

	UPROPERTY(Replicated)
		TArray<FARDragDropInfo> LootableItems;


	void RemoveItem(FName ItemID);
	//It will attemp to add ALL items from pick to player inventory.
	//For testing now, might use it later.

	void GiveItemToPlayer(FARDragDropInfo ItemIn, class AARPlayerController* PickupInstigator, int32 SlotID);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerGiveItemToPlayer(FARDragDropInfo ItemIn, class AARPlayerController* PickupInstigator, int32 SlotID);

	void GiveAllItemsTo(class AARPlayerController* PickupInstigator);
};



