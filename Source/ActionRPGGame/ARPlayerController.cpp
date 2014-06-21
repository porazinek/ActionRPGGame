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
	//bReplicates = true;
	//bOnlyRelevantToOwner = false;

	if (PlayerCameraManager)
	{
		PlayerCameraManager->ViewPitchMax = 70.0f;
	}
	MaxInventorySize = 50;
	Inventory.Reserve(50);
	
	for (int i = 0; i < MaxInventorySize; i++)
	{
		FARItemInfo item;
		item.ItemID = i;
		Inventory.Add(item);
	}
	//Inventory.AddZeroed(MaxInventorySize);
}

void AARPlayerController::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AARPlayerController, Inventory, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AARPlayerController, MaxInventorySize, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(AARPlayerController, InventoryObj, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AARPlayerController, InventorySmall, COND_OwnerOnly);

}
void AARPlayerController::OnRep_InventoryChanged()
{
	IsInventoryChanged = true;
}

void AARPlayerController::AddItemToInventory(FARItemInfo& Item)
{
	if (Inventory.Num() <= MaxInventorySize)
	{
		for (FARItemInfo& item : Inventory)
		{
			if (item.ItemName == "Default")
			{
				Item.ItemID = Inventory.Num() + Item.ItemID + 1;
				if (item.ItemType)
				{
					AARCharacter* MyChar = Cast<AARCharacter>(GetPawn());
					//AARCharacter* MyChar = Cast<AARCharacter>(GetOuterAARPlayerController()->GetPawn());
					FActorSpawnParameters SpawnInfo;
					SpawnInfo.bNoCollisionFail = true;
					SpawnInfo.Owner = MyChar;
					Item.Item = GetWorld()->SpawnActor<AARWeapon>(item.ItemType, SpawnInfo);
					Item.Item->SetOwner(MyChar);
					Item.Item->Instigator = MyChar;
					Item.Item->ItemOwner = MyChar;
				}

				item = Item;
				return;
			}
		}
	}
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
	OnRep_InventoryChanged();
}