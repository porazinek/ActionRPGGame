// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../Types/AREnumTypes.h"
#include "../Types/ARStructTypes.h"

#include "Net/UnrealNetwork.h"

#include "ARInventoryComponent.h"

UARInventoryComponent::UARInventoryComponent(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	bWantsInitializeComponent = true;
	bReplicates = true;
	SetActive(true);

	MaxInventorySize = 60;
	InitialInventorySize = 10;
	IsInventoryChanged = false;

}

void UARInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();
	bReplicates = true;
	SetActive(true);
	bAutoRegister = true;
	if (GetOwnerRole() < ROLE_Authority)
		return;

	Inventory.Reserve(MaxInventorySize);
	for (int32 CurrentSlot = 0; CurrentSlot < InitialInventorySize; CurrentSlot++)
	{
		FARDragDropInfo Slot;
		Slot.SlotIndex = CurrentSlot;
		Slot.ItemKey = NAME_None;
		Slot.ItemIndex = INDEX_NONE;
		Slot.ItemSlot = EItemSlot::Item_Inventory;
		Slot.DragDropSlot = EDragDropSlot::Inventory;
		Inventory.Add(Slot);
	}
	IsInventoryChanged = true;
}

void UARInventoryComponent::OnRegister()
{
	Super::OnRegister();
}

void UARInventoryComponent::OnRep_InventoryChanged()
{
	IsInventoryChanged = true;
}


void UARInventoryComponent::AddPickItemToInventory(FARDragDropInfo Item)
{
	if (Inventory.Num() <= MaxInventorySize)
	{
		for (FARDragDropInfo& item : Inventory)
		{
			if (item.ItemKey.IsNone())
			{
				item.ItemKey = Item.ItemKey;
				item.ItemSlot = Item.ItemSlot;
				item.ItemIndex = Item.ItemIndex;
				item.DragDropSlot = Item.DragDropSlot;
				PossesedItems.Add(item);
				ClientSetInventoryChanged();
				return;
			}
		}
	}
}
/*
	this should NEVER be called from client. EVER!.
*/
void UARInventoryComponent::AddItemToInventory(FARDragDropInfo Item)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerAddItemToInventory(Item);
		//IsInventoryChanged = true;
	}
	else
	{
		if (Inventory.Num() <= MaxInventorySize)
		{
			for (FARDragDropInfo& item : Inventory)
			{
				if (item.ItemKey.IsNone())
				{
					item.ItemKey = Item.ItemKey;
					item.ItemSlot = Item.ItemSlot;
					item.ItemIndex = Item.ItemIndex;
					PossesedItems.Add(item);
					ClientSetInventoryChanged();
					//ClientSetInventoryChanged();
					return;
				}
			}
		}
	}
}

void UARInventoryComponent::ClientSetInventoryChanged_Implementation()
{
	IsInventoryChanged = true;
}

void UARInventoryComponent::ServerAddItemToInventory_Implementation(FARDragDropInfo Item)
{
	AddItemToInventory(Item);
}
bool UARInventoryComponent::ServerAddItemToInventory_Validate(FARDragDropInfo Item)
{
	return true;
}

void UARInventoryComponent::AddItemToInventoryOnSlot(FARDragDropInfo Item, int32 SlotID)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerAddItemToInventoryOnSlot(Item, SlotID);
	}
	else
	{
		if (Inventory.Num() <= MaxInventorySize)
		{
			for (FARDragDropInfo& item : Inventory)
			{//can be optimized by using indexes directly
				if (item.SlotIndex == SlotID && item.ItemKey != NAME_None)
				{
					FARDragDropInfo oldItemTemp = item;
					item.ItemKey = Item.ItemKey;
					item.ItemSlot = Item.ItemSlot;
					item.ItemIndex = Item.ItemIndex;
					item.DragDropSlot == EDragDropSlot::Inventory;
					for (FARDragDropInfo& oldItem : Inventory)
					{
						if (Item.SlotIndex == oldItem.SlotIndex)
						{
							oldItem.ItemKey = oldItemTemp.ItemKey;
							oldItem.ItemSlot = oldItemTemp.ItemSlot;
							oldItem.ItemIndex = oldItemTemp.ItemIndex;
							IsInventoryChanged = true;
							ClientSetInventoryChanged();
							return;
						}
					}
					IsInventoryChanged = true;
					ClientSetInventoryChanged();
					return;
				}
				if (item.ItemKey == NAME_None && item.SlotIndex == SlotID)
				{
					item.ItemKey = Item.ItemKey;
					item.ItemSlot = Item.ItemSlot;
					item.ItemIndex = Item.ItemIndex;
					item.DragDropSlot == EDragDropSlot::Inventory;

					if (Item.DragDropSlot == EDragDropSlot::Inventory)
						RemoveItemFromInventory(Item.ItemKey, Item.OldSlotIndex);
					


					IsInventoryChanged = true;
					ClientSetInventoryChanged();
					return;
				}
			}
		}
	}
}
void UARInventoryComponent::ServerAddItemToInventoryOnSlot_Implementation(FARDragDropInfo Item, int32 SlotID)
{
	AddItemToInventoryOnSlot(Item, SlotID);
}
bool UARInventoryComponent::ServerAddItemToInventoryOnSlot_Validate(FARDragDropInfo Item, int32 SlotID)
{
	int32 ItemCount = PossesedItems.Num();
	int32 CurrentIndex = 0;
	/*
	Since it is used to swap items, we check it user have particular item in inventory.
	If he dosn't then he probably is trying to cheat.
	*/
	for (FARDragDropInfo& item : PossesedItems)
	{
		//check if incoming item, is indeed in possesion of player.
		if (Item.ItemIndex == item.ItemIndex)
		{ 
			if (Item.OldSlotIndex == INDEX_NONE)
				return false;
			return true;
		}
	}


	return false;
}

bool UARInventoryComponent::RemoveItemFromInventory(FName ItemID, int32 SlotID)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerRemoveItemFromInventory(ItemID, SlotID);
	}
	else
	{
		for (FARDragDropInfo& item : Inventory)
		{
			if (item.SlotIndex == SlotID && item.ItemKey != NAME_None)
			{
				//we don't remove actually anything from array.
				//just change ID and slot types, to match an "empty" slot 
				// in inventory.
				item.ItemKey = NAME_None;
				item.ItemIndex = INDEX_NONE;
				item.ItemSlot = EItemSlot::Item_Inventory;
				item.DragDropSlot = EDragDropSlot::Inventory;
				IsInventoryChanged = true;
				ClientSetInventoryChanged();
				return true;
			}
		}
	}
	return false;
}
void UARInventoryComponent::ServerRemoveItemFromInventory_Implementation(FName ItemID, int32 SlotID)
{
	RemoveItemFromInventory(ItemID, SlotID);
}
bool UARInventoryComponent::ServerRemoveItemFromInventory_Validate(FName ItemID, int32 SlotID)
{
	for (FARDragDropInfo& item : PossesedItems)
	{
		if (ItemID == item.ItemKey)
		{
			return true;
		}
	}
	return false;
}

void UARInventoryComponent::RemoveItem(FName ItemID)
{

}

void UARInventoryComponent::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UARInventoryComponent, MaxInventorySize, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UARInventoryComponent, Inventory, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UARInventoryComponent, IsInventoryChanged, COND_OwnerOnly);
}
