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
		FInventorySlot Slot;
		Slot.SlotID = CurrentSlot;
		Slot.ItemID = NAME_None;
		Slot.ItemSlot = EItemSlot::Item_Inventory;
		Slot.EEquipmentSlot = EEquipmentSlot::Item_Inventory;
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

void UARInventoryComponent::AddItemToInventory(FInventorySlot Item)
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
			for (FInventorySlot& item : Inventory)
			{
				if (item.ItemID.IsNone())
				{
					item.ItemID = Item.ItemID;
					item.ItemSlot = Item.ItemSlot;
					item.EEquipmentSlot = Item.EEquipmentSlot;
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

void UARInventoryComponent::ServerAddItemToInventory_Implementation(FInventorySlot Item)
{
	AddItemToInventory(Item);
}
bool UARInventoryComponent::ServerAddItemToInventory_Validate(FInventorySlot Item)
{
	return true;
}

void UARInventoryComponent::AddItemToInventoryOnSlot(FInventorySlot Item, int32 SlotID)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerAddItemToInventoryOnSlot(Item, SlotID);
	}
	else
	{
		if (Inventory.Num() <= MaxInventorySize)
		{
			for (FInventorySlot& item : Inventory)
			{
				if (item.SlotID == SlotID && item.ItemID != NAME_None)
				{
					FInventorySlot oldItemTemp = item;
					item.ItemID = Item.ItemID;
					item.ItemSlot = Item.ItemSlot;
					item.EEquipmentSlot = Item.EEquipmentSlot;
					for (FInventorySlot& oldItem : Inventory)
					{
						if (Item.SlotID == oldItem.SlotID)
						{
							oldItem.ItemID = oldItemTemp.ItemID;
							oldItem.ItemSlot = oldItemTemp.ItemSlot;
							oldItem.EEquipmentSlot = oldItemTemp.EEquipmentSlot;
							IsInventoryChanged = true;
							ClientSetInventoryChanged();
							return;
						}
					}
					IsInventoryChanged = true;
					ClientSetInventoryChanged();
					return;
				}
				if (item.ItemID == NAME_None && item.SlotID == SlotID)
				{
					item.ItemID = Item.ItemID;
					item.ItemSlot = Item.ItemSlot;
					item.EEquipmentSlot = Item.EEquipmentSlot;

					if (Item.EEquipmentSlot == EEquipmentSlot::Item_Inventory)
						RemoveItemFromInventory(Item.ItemID, Item.OldSlotID);
					
					IsInventoryChanged = true;
					ClientSetInventoryChanged();
					return;
				}
			}
		}
	}
}
void UARInventoryComponent::ServerAddItemToInventoryOnSlot_Implementation(FInventorySlot Item, int32 SlotID)
{
	AddItemToInventoryOnSlot(Item, SlotID);
}
bool UARInventoryComponent::ServerAddItemToInventoryOnSlot_Validate(FInventorySlot Item, int32 SlotID)
{
	for (FInventorySlot& item : PossesedItems)
	{
		if (Item.ItemID == item.ItemID)
		{
			return true;
		}
	}
	return false;
	/*
	Since it is used to swap items, we check it user have particular item in inventory.
	If he dosn't then he probably is trying to cheat.
	*/
}

bool UARInventoryComponent::RemoveItemFromInventory(FName ItemID, int32 SlotID)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerRemoveItemFromInventory(ItemID, SlotID);
	}
	else
	{
		for (FInventorySlot& item : Inventory)
		{
			if (item.SlotID == SlotID && item.ItemID != NAME_None)
			{
				//we don't remove actually anything from array.
				//just change ID and slot types, to match an "empty" slot 
				// in inventory.
				item.ItemID = NAME_None;
				item.ItemSlot = EItemSlot::Item_Inventory;
				item.EEquipmentSlot = EEquipmentSlot::Item_Inventory;
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
	for (FInventorySlot& item : PossesedItems)
	{
		if (ItemID == item.ItemID)
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
