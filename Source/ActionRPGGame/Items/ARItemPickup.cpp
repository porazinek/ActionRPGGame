// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "ActionRPGGame.h"
#include "../Types/ARStructTypes.h"
#include "../Types/ARItemTypes.h"
#include "../ARPlayerController.h"
#include "ARInventoryComponent.h"

#include "ARItemDataAsset.h"

#include "Net/UnrealNetwork.h"

#include "ARItemPickup.h"

AARItemPickup::AARItemPickup(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	bReplicates = true;
	bNetUseOwnerRelevancy = false;
}

void AARItemPickup::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (ItemContainer.ListOfItems.Num() > 0)
	{
		for (FARItemPickupCont& cont : ItemContainer.ListOfItems)
		{
			if (cont.ItemData)
			{
				for (int32 idx : cont.ItemsList)
				{
					FARDragDropInfo tempSlot;
					FItemEntry* tempItem = cont.ItemData->GetItemFromArrayPtr(idx);
					tempSlot.ItemKey = tempItem->Key;
					tempSlot.ItemIndex = tempItem->Index;
					tempSlot.ItemSlot = tempItem->ItemDataInfo.ItemSlot;
					tempSlot.IsAttached = false;
					tempSlot.DragDropSlot = EDragDropSlot::LootWindow;
					LootableItems.Add(tempSlot);
				}
			}
		}
	}
}

void AARItemPickup::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AARItemPickup, LootableItems)
}


void AARItemPickup::RemoveItem(FName ItemID)
{
	for (FARDragDropInfo& item : LootableItems)
	{
		if (item.ItemKey == ItemID)
		{
			LootableItems.RemoveSingle(item);
		}
	}
}

void AARItemPickup::GiveItemToPlayer(FARDragDropInfo ItemIn, class AARPlayerController* PickupInstigator, int32 SlotID)
{
	if (Role < ROLE_Authority)
	{
		ServerGiveItemToPlayer(ItemIn, PickupInstigator, SlotID);
	}
	else
	{
		if (!PickupInstigator)
			return;

		UARInventoryComponent* InvComp = PickupInstigator->Inventory;

		if (InvComp)
		{
			InvComp->AddItemToInventoryOnSlot(ItemIn, SlotID);
		}
	}
}

void AARItemPickup::ServerGiveItemToPlayer_Implementation(FARDragDropInfo ItemIn, class AARPlayerController* PickupInstigator, int32 SlotID)
{
	GiveItemToPlayer(ItemIn, PickupInstigator, SlotID);
}

bool AARItemPickup::ServerGiveItemToPlayer_Validate(FARDragDropInfo ItemIn, class AARPlayerController* PickupInstigator, int32 SlotID)
{
	for (FARDragDropInfo& item : LootableItems)
	{
		if (ItemIn.ItemKey == item.ItemKey)
		{
			return true;
		}
	}
	return false;
}


/*
	Remove items from container.
	Destroy actor after all items has been looted (or after time passed)
	Add looting widget, to manually  pickup items to inventory.
	*/
void AARItemPickup::GiveAllItemsTo(class AARPlayerController* PickupInstigator)
{
	SetOwner(PickupInstigator);
	//if (!PickupInstigator) //really shouldn't be ever null.
	//	return;
	////construct LootableItems while looting or while spawning ?
	//if (ItemContainer.ListOfItems.Num() > 0)
	//{
	//	UARInventoryComponent* InvComp = PickupInstigator->Inventory;

	//	if (InvComp)
	//	{
	//		for (FARItemPickupCont& cont : ItemContainer.ListOfItems)
	//		{
	//			if (cont.ItemData)
	//			{
	//				for (int32 idx : cont.ItemsList)
	//				{
	//					FInventorySlot tempSlot;
	//					FItemEntry* tempItem = cont.ItemData->GetItemFromArrayPtr(idx);
	//					tempSlot.ItemID = tempItem->Key;
	//					tempSlot.ItemIndex = tempItem->Index;
	//					tempSlot.ItemSlot = tempItem->ItemDataInfo.ItemSlot;
	//					tempSlot.IsAttached = false;
	//					LootableItems.Add(tempSlot);

	//				//	InvComp->AddPickItemToInventory(tempSlot);
	//				}
	//			}
	//		}
	//	}
	//}
}

