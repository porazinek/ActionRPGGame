// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "ActionRPGGame.h"

#include "Componenets/AREquipmentComponent.h"
#include "ARCharacter.h"
#include "Items/ARWeapon.h"
//#include "../Camera/PlayerCameraManager.h"

#include "Net/UnrealNetwork.h"

#include "ARPlayerController.h"

AARPlayerController::AARPlayerController(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

	InventoryVisibility = EVisibility::Collapsed;
	CharacterSheetVisibility = EVisibility::Collapsed;
	//bReplicates = true;
	//bOnlyRelevantToOwner = false;
	IsInventoryChanged = false;
	if (PlayerCameraManager)
	{
		PlayerCameraManager->ViewPitchMax = 70.0f;
	}
	MaxInventorySize = 4;
	Inventory.Reserve(MaxInventorySize);
	InventorySmall.Reserve(MaxInventorySize);
	for (int32 i = 0; i < MaxInventorySize; i++)
	{
		FInventorySlot slot;
		slot.SlotID = i;
		slot.ItemID = "-1"; //no item on that slot.
		slot.ItemSlot = EItemSlot::Item_Inventory;
		slot.EEquipmentSlot = EEquipmentSlot::Item_Inventory;
		InventorySmall.Add(slot);
	};
	for (int32 i = 0; i < MaxInventorySize; i++)
	{
		FARItemInfo item;
		item.ItemID = i;
		Inventory.Add(item);
	}
	PlayerCameraManagerClass = AARPlayerCameraManager::StaticClass();

	//Inventory.AddZeroed(MaxInventorySize);
}

void AARPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//GUI input
	InputComponent->BindAction("ShowInventory", IE_Pressed, this, &AARPlayerController::SetInventoryVisibility);
	InputComponent->BindAction("ShowCharacterSheet", IE_Pressed, this, &AARPlayerController::SetCharacterSheetVisibility);
}
void AARPlayerController::SetInventoryVisibility()
{
	if (InventoryVisibility == EVisibility::Collapsed)
	{
		InventoryVisibility = EVisibility::Visible;
	}
	else
	{
		InventoryVisibility = EVisibility::Collapsed;
	}
}

void AARPlayerController::SetCharacterSheetVisibility()
{
	if (CharacterSheetVisibility == EVisibility::Collapsed)
	{
		CharacterSheetVisibility = EVisibility::Visible;
	}
	else
	{
		CharacterSheetVisibility = EVisibility::Collapsed;
	}
}
void AARPlayerController::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AARPlayerController, Inventory, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AARPlayerController, MaxInventorySize, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(AARPlayerController, InventoryObj, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AARPlayerController, InventorySmall, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AARPlayerController, IsInventoryChanged, COND_OwnerOnly);
}
void AARPlayerController::OnRep_InventoryChanged()
{
	IsInventoryChanged = true;
}

void AARPlayerController::AddItemToInventory(FInventorySlot Item)
{
	if (Role < ROLE_Authority)
	{
		ServerAddItemToInventory(Item);
		//IsInventoryChanged = true;
	}
	else
	{
		if (InventorySmall.Num() <= MaxInventorySize)
		{
			for (FInventorySlot& item : InventorySmall)
			{
				if (item.ItemID == "-1")
				{
					item.ItemID = Item.ItemID;
					item.ItemSlot = Item.ItemSlot;
					item.EEquipmentSlot = Item.EEquipmentSlot;
					//ClientSetInventoryChanged();
					return;
				}
			}
		}
	}
}

void AARPlayerController::ServerAddItemToInventory_Implementation(FInventorySlot Item)
{
	AddItemToInventory(Item);
}
bool AARPlayerController::ServerAddItemToInventory_Validate(FInventorySlot Item)
{
	//I dunno how I can validate this.
	//best option would be to never call this directely on client. Just do it indirectly.
	return true;
}
//it's really only used to swap items.
void AARPlayerController::AddItemToInventoryOnSlot(FInventorySlot Item, int32 SlotID)
{
	if (Role < ROLE_Authority)
	{
		ServerAddItemToInventoryOnSlot(Item, SlotID);
	}
	else
	{
		if (InventorySmall.Num() <= MaxInventorySize)
		{
			for (FInventorySlot& item : InventorySmall)
			{
				if (item.SlotID == SlotID && item.ItemID != "-1")
				{
					FInventorySlot oldItemTemp = item;
					item.ItemID = Item.ItemID;
					item.ItemSlot = Item.ItemSlot;
					item.EEquipmentSlot = Item.EEquipmentSlot;
					for (FInventorySlot& oldItem : InventorySmall)
					{
						if (Item.SlotID == oldItem.SlotID)
						{
							oldItem.ItemID = oldItemTemp.ItemID;
							oldItem.ItemSlot = oldItemTemp.ItemSlot;
							oldItem.EEquipmentSlot = oldItemTemp.EEquipmentSlot;
							ClientSetInventoryChanged();
							//OnRep_InventoryChanged();
							return;
						}
					}
					IsInventoryChanged = true;
					//OnRep_InventoryChanged();
					return;
				}
				if (item.ItemID == "-1" && item.SlotID == SlotID)
				{
					item.ItemID = Item.ItemID;
					item.ItemSlot = Item.ItemSlot;
					item.EEquipmentSlot = Item.EEquipmentSlot;
					ClientSetInventoryChanged();
					//RemoveItemFromInventory(Item.ItemID, Item.SlotID);
					//OnRep_InventoryChanged();
					return;
				}
			}
		}
	}
}
void AARPlayerController::ServerAddItemToInventoryOnSlot_Implementation(FInventorySlot Item, int32 SlotID)
{
	AddItemToInventoryOnSlot(Item, SlotID);
}
bool AARPlayerController::ServerAddItemToInventoryOnSlot_Validate(FInventorySlot Item, int32 SlotID)
{
	for (FInventorySlot& item : InventorySmall)
	{
		if (item.ItemID == item.ItemID)
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

bool AARPlayerController::RemoveItemFromInventory(FName ItemID, int32 SlotID)
{
	if (Role < ROLE_Authority)
	{
		ServerRemoveItemFromInventory(ItemID, SlotID);
	}
	else
	{
		for (FInventorySlot& item : InventorySmall)
		{
			if (item.SlotID == SlotID)
			{
				//we don't remove actually anything from array.
				//just change ID and slot types, to match an "empty" slot 
				// in inventory.
				item.ItemID = "-1";
				item.ItemSlot = EItemSlot::Item_Inventory;
				item.EEquipmentSlot = EEquipmentSlot::Item_Inventory;
				ClientSetInventoryChanged();
				return true;
			}
		}
	}
	return false;
}
void AARPlayerController::ServerRemoveItemFromInventory_Implementation(FName ItemID, int32 SlotID)
{
	RemoveItemFromInventory(ItemID, SlotID);
}
bool AARPlayerController::ServerRemoveItemFromInventory_Validate(FName ItemID, int32 SlotID)
{
	return true;
}

void AARPlayerController::SwapItemPosition(FARItemInfo& Item, int32 NewIndex)
{
	int32 index = 0;
	for (FARItemInfo& item : Inventory)
	{
		if (item.ItemID == Item.ItemID)
		{
			index = 0;
			break;
		}
		index++;
	}
	int32 OldIndex = Inventory.Find(Item); //find index of old item.

	FARItemInfo& oldItem = Inventory[NewIndex]; //find item that is on NewIndex.

	Inventory.Swap(NewIndex, index);
	//OnRep_InventoryChanged();
}

void AARPlayerController::ClientSetInventoryChanged_Implementation()
{
	IsInventoryChanged = true;
}

void AARPlayerController::SetInventoryChanged()
{
	if (Role < ROLE_Authority)
	{
		ServerSetInventoryChanged();
	}
	else
	{
		IsInventoryChanged = false;
	}
}
void AARPlayerController::ServerSetInventoryChanged_Implementation()
{
	SetInventoryChanged();
}
bool AARPlayerController::ServerSetInventoryChanged_Validate()
{
	return true;
}