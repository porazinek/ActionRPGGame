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
	bNetUseOwnerRelevancy = true;
	
}

//void AARItemPickup::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//}

void AARItemPickup::GiveAllItemsTo(class AARPlayerController* PickupInstigator)
{
	if (!PickupInstigator) //really shouldn't be ever null.
		return;

	if (ItemContainer.ListOfItems.Num() > 0)
	{
		UARInventoryComponent* InvComp = PickupInstigator->Inventory;

		if (InvComp)
		{
			for (FARItemPickupCont& cont : ItemContainer.ListOfItems)
			{
				if (cont.ItemData)
				{
					for (int32 idx : cont.ItemsList)
					{
						FInventorySlot tempSlot;
						FItemEntry* tempItem = cont.ItemData->GetItemFromArrayPtr(idx);
						tempSlot.ItemID = tempItem->Key;
						tempSlot.ItemIndex = tempItem->Index;
						tempSlot.ItemSlot = tempItem->ItemDataInfo.ItemSlot;
						tempSlot.IsAttached = false;
						
						InvComp->AddPickItemToInventory(tempSlot);
					}
				}
			}
		}
	}
}

