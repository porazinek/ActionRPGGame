// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "ActionRPGGame.h"
#include "../Types/ARStructTypes.h"
#include "../ARPlayerController.h"

#include "ARItemDataAsset.h"

#include "Net/UnrealNetwork.h"

#include "ARItemPickup.h"

AARItemPickup::AARItemPickup(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	bReplicates = true;
	bNetUseOwnerRelevancy = true;

}


void AARItemPickup::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARItemPickup, ItemPickups);
}

void AARItemPickup::GiveAllItemsTo(class AARPlayerController* PickupInstigator)
{
	if (!PickupInstigator) //really shouldn't be ever null.
		return;

	//probably need to change. Item pickip shouldn't check for it. Just pass items to
	//controller directly and controller will take care of checking and returning bool.
	if (PickupInstigator->Inventory.Num() <= PickupInstigator->MaxInventorySize)
	{
		//for (FPickupItem& itemPickup : ItemPickups)
		for (auto it = ItemPickups.CreateIterator(); it; ++it)
		{
			if (it->ItemSlot == EItemSlot::Item_Chest)
			{
				FString usless;
				FARItemData* data = ChestItemDataTable->FindRow<FARItemData>(it->ItemID, usless);
				if (data)
				{
					FInventorySlot item;
					item.ItemID = it->ItemID;
					item.ItemSlot = it->ItemSlot;
					item.EEquipmentSlot = it->EEquipmentSlot;
					PickupInstigator->AddItemToInventory(item);
					ItemPickups.RemoveAt(it.GetIndex()); //remove item
					--it;
					break;
				}
			}

			//if (it->DataAsset.IsValid())
			//{
			//	for (FARItemInfo& item : it->DataAsset->Items)
			//	{
			//		if (item.ItemName == it->ItemName)
			//		{
			//			//PickupInstigator->AddItemToInventory(item);
			//			ItemPickups.RemoveAt(it.GetIndex()); //remove item
			//			--it;
			//			break;
			//			//if (ItemPickups.Num() == 1)
			//			//{
			//			//	--it;
			//			//}
			//		}
			//	}
			//}
		}
	}

	if (ItemPickups.Num() <= 0)
	{
		Destroy(false, true);
	}
}