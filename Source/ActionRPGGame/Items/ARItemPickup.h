// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "../Types/ARStructTypes.h"

#include "ARItemPickup.generated.h"

/*
	Base class for item pickups.

	I'm not sure how exactly data for item pick will be stored in that class. But proposed usage it as fallows:

	1. Designer create blueprint version of this class.
	2. Setup list of item it contain.
	3. Place it on level.
	4. Or it can drop from other actor.
	5. Player come to pickup, press interaction button, and is presented with menu to pick items.


	Things to consider:
	1. Store entire FAARItemInfo in Array
	2. Or just w FNames/Ints for lookup in DataAsset ?
	3. Items contained in this actor need some visual repsentation in menu. Like icon.
	4. They also need info. Like stats or value.
	5. I probably need better struct structure.


	After a second thought it will be best to simply store Ints/FNames here.
	When User Interactr with actor it will just query DataAsset to pickup whatever items it have
	and then send them to slate.

	Hmm. But since we have separate DataAssets for each item type, how do we know, from which data asset we want item ?
	Maybe something like this 
	struct FPickupItem
	{
		TWeakObjectPtr<UDataAsset> DataAssetWithItem;
		FName ItemName;
	}
	TArray<FPickupItem> ItemsToPick;

	And wHen Open like
	TArray<FARItemInfo> ItemsInfo
	void OnInteraction
	{
		for(FPickupItem& itemPickup : ItemsToPick)
		{
			for(FARItemInfo& item : itemPickup.DataAssetWithItem->Items)
			{
				if(item == ItemName)
				{
					ItemsInfo.Add(item) // and then show widget, which will pull data from ItemsInfo Array.
					//after user pickup item, clear this array, and destroy actor. Or leave and let it respawn items.
					//depends on scenario.
					//we can also omnit widget all togather and just put item to player inventory.
					//assuming player have space in it.
				}
			}
		}
	}
*/

UCLASS()
class AARItemPickup : public AActor
{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY(EditAnywhere, Replicated, Category="Pickup Items")
		TArray<FInventorySlot> ItemPickups;

	//It will attemp to add ALL items from pick to player inventory.
	//For testing now, might use it later.
	void GiveAllItemsTo(class AARPlayerController* PickupInstigator);

protected:

private:
};



