// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "ActionRPGGame.h"

#include "../Types/AREnumTypes.h"
#include "../Items/ARItem.h"
#include "../Items/ARWeapon.h"
#include "../Items/ARArmor.h"
#include "../ARCharacter.h"
#include "../Items/ARItemDataAsset.h"
#include "../ARSingleton.h"
#include "../Types/ARInvItem.h"
#include "../ARPlayerController.h"
#include "../Items/ARItemPickup.h"
#include "../BlueprintLibrary/ARTraceStatics.h"

#include "Net/UnrealNetwork.h"

#include "AREquipmentComponent.h"



UAREquipmentComponent::UAREquipmentComponent(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	bReplicates = true;
	bWantsInitializeComponent = true;
	bAutoRegister = true;
	//ChestItem = "-1";
}

void UAREquipmentComponent::InitializeComponent()
{
	Super::InitializeComponent();
}
void UAREquipmentComponent::BeginDestroy()
{
	Super::BeginDestroy();
	//InventorySlate.Reset();
}
void UAREquipmentComponent::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAREquipmentComponent, TargetCharacter);
	DOREPLIFETIME(UAREquipmentComponent, TargetController);

	//DOREPLIFETIME(UAREquipmentComponent, IsInventoryChanged);

	DOREPLIFETIME_CONDITION(UAREquipmentComponent, Inventory, COND_OwnerOnly);
	//DOREPLIFETIME(UAREquipmentComponent, InventoryData);

	DOREPLIFETIME(UAREquipmentComponent, ActiveLeftHandWeapon);
	DOREPLIFETIME(UAREquipmentComponent, ActiveRightHandWeapon);

	DOREPLIFETIME(UAREquipmentComponent, ChestItem);
}

void UAREquipmentComponent::OnRep_AtiveLeftHandWeapon()
{
	SetAttachWeapon(ActiveLeftHandWeapon, LeftWeaponSocket);
}
void UAREquipmentComponent::OnRep_ActiveRightHandWeapon()
{
	SetAttachWeapon(ActiveRightHandWeapon, RightWeaponSocket);
}
void UAREquipmentComponent::OnRep_ChestItem()
{
	//if (ChestItem.ItemMesh.IsValid())
	//{
	//SetChestMesh(ChestItem.ItemMesh);
	//}

	ChangeChestItem(ChestItem);
}
void UAREquipmentComponent::SetChestMesh(TAssetPtr<USkeletalMesh> MeshToSet)
{
	//if (ChestMeshToLoad.ResolveObject() != nullptr)
	//{
	//UObject* NewChestMesh = ChestMeshToLoad.ResolveObject();

	//should change it to Withing, but for testing.. it will do just fine.

	AARCharacter* CharOwner = Cast<AARCharacter>(GetOwner());
	if (CharOwner)
	{
		CharOwner->ChestMesh->SetSkeletalMesh(Cast<USkeletalMesh>(MeshToSet.Get()));
	}
	//}
}

void UAREquipmentComponent::OnRep_Inventory()
{
	//IsInventoryChanged = true;
}

//terrible sorting but:
// 1. It should work for now
// 2. Sorted array will have at most 20 elements.
// 3. But the question is how often we will sort that array ?
// 4. If item have for example 4 stats, the array will need to be sorted 4 times, each time for each stat on item.
void UAREquipmentComponent::SortEquipedItemsByAttribute(FName AttributeName)
{
	//for (int32 itemIdx = 0; itemIdx < EquippedItems.Num(); itemIdx++)
	//{
	//	for (int32 i = 0; i < EquippedItems.Num() - 1; i++)
	//	{
	//		for (int32 attrIdx = 0; attrIdx < EquippedItems[itemIdx].Attributes.Num(); attrIdx++)
	//		{
	//			if (EquippedItems[i].Attributes[attrIdx].AttributeName == AttributeName)
	//			{
	//				if (EquippedItems[i].Attributes[attrIdx].ModValue < EquippedItems[i + 1].Attributes[attrIdx].ModValue)
	//				{
	//					FARItemInfo tempArr = EquippedItems[i + 1];
	//					EquippedItems[i + 1] = EquippedItems[i];
	//					EquippedItems[i] = tempArr;
	//				}
	//			}
	//		}
	//	}
	//}
}
/* Inventory Handling **/
void UAREquipmentComponent::PickupItem()
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerPickupItem();
	}
	else
	{
		FHitResult HitResult = UARTraceStatics::GetHitResult(1000.0f, "PickItem", TargetCharacter, false, false, EARTraceType::Trace_UI);

		if (HitResult.GetActor())
		{
			AARItemPickup* pickedItem = Cast<AARItemPickup>(HitResult.GetActor());
			{
				if (pickedItem)
				{
					pickedItem->GiveAllItemsTo(TargetController);
				}
			}
		}
	}
}
void UAREquipmentComponent::ServerPickupItem_Implementation()
{
	PickupItem();
}

bool UAREquipmentComponent::ServerPickupItem_Validate()
{
	return true;
}
void UAREquipmentComponent::AddItemToInventory(FInventorySlot NewItem)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerAddItemToInventory(NewItem);
	}
	else
	{
		//if (TargetController)
		//{
		//	if (ItemSlot == EItemSlot::Item_Chest)
		//	{
		//		UARItemDataAsset* ItemDataAsset = Cast<UARItemDataAsset>(StaticLoadObject(UARItemDataAsset::StaticClass(), NULL, *ChestItemDataAssetPath, NULL, LOAD_None, NULL));

		//		if (ItemDataAsset && ItemDataAsset->Items.Num() > 0)
		//		{
		//			for (FARItemInfo& item : ItemDataAsset->Items)
		//			{
		//				if (item.ItemID == ItemID)
		//				{
		//					FInventorySlot it;
		//					it.ItemID = item.ItemID;
		//					it.ItemSlot = item.ItemSlot;

		//					TargetController->InventorySmall.Add(it);
		//					//TargetController->Inventory.Add(item);
		//					break;
		//				}
		//			}
		//		}
		//	}
		//}
	}
}
void UAREquipmentComponent::ServerAddItemToInventory_Implementation(FInventorySlot NewItem)
{
	if (TargetController)
	{
		if (NewItem.ItemSlot == EItemSlot::Item_Chest)
		{
			FString usless;
			FARItemData* data = ChestItemDataTable->FindRow<FARItemData>(NewItem.ItemID, usless);
			if (data)
			{
				TargetController->AddItemToInventory(NewItem);
			}
		}
		if (NewItem.ItemSlot == EItemSlot::Item_Weapon)
		{
			FString usless;
			FARItemData* data = WeaponItemDataTable->FindRow<FARItemData>(NewItem.ItemID, usless);
			if (data)
			{
				TargetController->AddItemToInventory(NewItem);
			}
		}
	}
}
bool UAREquipmentComponent::ServerAddItemToInventory_Validate(FInventorySlot NewItem)
{
	return true;
}
TArray<FARItemInfo> UAREquipmentComponent::GetItems()
{
	return Inventory;
}

void UAREquipmentComponent::SetAttachWeapon(class AARWeapon* Weapon, FName SocketName)
{
	if (Weapon)
	{
		AARCharacter* MyChar = Cast<AARCharacter>(GetOwner());
		if (MyChar)
		{
			Weapon->WeaponMesh->AttachTo(MyChar->Mesh, SocketName);
			Weapon->WeaponMesh->SetHiddenInGame(false);
		}
		Weapon->WeaponOwner = MyChar;
	}
}

void UAREquipmentComponent::ServerChangeItem_Implementation(FInventorySlot ItemIn, int32 OldItemSlotID)
{
	ChangeItem(ItemIn, OldItemSlotID);
}
bool UAREquipmentComponent::ServerChangeItem_Validate(FInventorySlot ItemIn, int32 OldItemSlotID)
{
	return true;
}
void UAREquipmentComponent::ChangeItem(FInventorySlot ItemIn, int32 OldItemSlotID)
{
	bool itemRemoved = false;
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerChangeItem(ItemIn, OldItemSlotID);
	}
	else
	{
		//check if there is something in that particular slot
		for (AARItem* eqItem : EquippedItems)
		{
			if (eqItem->ItemSlotEquipped == ItemIn.ItemSlot)
			{
				FInventorySlot tempItem;
				tempItem.ItemID = eqItem->ItemID;
				tempItem.ItemSlot = eqItem->ItemSlotEquipped;
				//if there is, Unequip it. Before we proceed.
				UnEquipItem(ItemIn);
				itemRemoved = TargetController->RemoveItemFromInventory(ItemIn.ItemID, ItemIn.SlotID);
				TargetController->AddItemToInventoryOnSlot(tempItem, OldItemSlotID);
			}
		}
		switch (ItemIn.EEquipmentSlot)
		{
		case EEquipmentSlot::Item_Chest:
		{
			if (ChangeChestItem(ItemIn))
			{
				if (!itemRemoved)
				{
					TargetController->RemoveItemFromInventory(ItemIn.ItemID, ItemIn.SlotID);
					break;
				}
			}
			break;
		}
		case EEquipmentSlot::Item_LeftHandOne:
		{
			AddLeftHandWeapon(ItemIn.ItemID);
		}
		default:
			break;
		}
	}
}

void UAREquipmentComponent::UnEquipItem(FInventorySlot ItemIn)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerUnEquipItem(ItemIn);
	}
	else
	{
		for (int32 index = 0; index < EquippedItems.Num(); index++)
		{
			if (EquippedItems[index]->ItemSlotEquipped == ItemIn.ItemSlot)
			{
				EquippedItems[index]->Destroy();
				EquippedItems.RemoveAt(index);
			}
		}
	}
}
void UAREquipmentComponent::ServerUnEquipItem_Implementation(FInventorySlot ItemIn)
{
	UnEquipItem(ItemIn);
}
bool UAREquipmentComponent::ServerUnEquipItem_Validate(FInventorySlot ItemIn)
{
	return true;
}

void UAREquipmentComponent::ServerChangeChestItem_Implementation(FInventorySlot ItemIn)
{
	ChangeChestItem(ItemIn);
}
bool UAREquipmentComponent::ServerChangeChestItem_Validate(FInventorySlot ItemIn)
{
	return true;
}
bool UAREquipmentComponent::ChangeChestItem(FInventorySlot ItemIn)
{
	FStreamableManager& Loader = UARSingleton::Get().AssetLoader;
	TArray<FStringAssetReference> ObjToLoad;

	//UBlueprint* gen = Cast<UBlueprint>(itemObj);
	//this can be called by both client and server
	//server need to do it, to propagate mesh change
	//to other players.
	//and client
	FString usless;
	FARItemData* data = ChestItemDataTable->FindRow<FARItemData>(ItemIn.ItemID, usless);

	if (data)
	{
		UBlueprint* gen = LoadObject<UBlueprint>(NULL, *data->ItemBlueprint.ToStringReference().ToString(), NULL, LOAD_None, NULL);
		if (!gen)
			return false;

		TArray<FStringAssetReference> ObjToLoad;
		FStreamableManager& Loader = UARSingleton::Get().AssetLoader;

		AARCharacter* MyChar = Cast<AARCharacter>(GetOwner());

		FActorSpawnParameters SpawnInfo;
		SpawnInfo.bNoCollisionFail = true;
		SpawnInfo.Owner = MyChar;
		AARArmor* itemBase = GetWorld()->SpawnActor<AARArmor>(gen->GeneratedClass, SpawnInfo);
		itemBase->SetOwner(MyChar);
		itemBase->Instigator = MyChar;
		itemBase->ItemID = ItemIn.ItemID;
		itemBase->ItemSlotEquipped = ItemIn.ItemSlot;
		if (itemBase)
		{
			for (int32 itemIdx = 0; itemIdx < EquippedItems.Num(); itemIdx++)
			{
				if (EquippedItems[itemIdx]->IsA(itemBase->GetClass()))
				{
					EquippedItems[itemIdx]->Destroy();
					EquippedItems.RemoveAt(itemIdx);
				}
			}

			EquippedItems.AddUnique(itemBase);
			ChestMeshToLoad = itemBase->ArmorMesh.ToStringReference();
			ObjToLoad.AddUnique(ChestMeshToLoad);
			Loader.RequestAsyncLoad(ObjToLoad, FStreamableDelegate::CreateUObject(this, &UAREquipmentComponent::DoAsyncChestChange));
			ChestItem = ItemIn;
			return true;
		}

		//this is only for server
		//because here we will be changing character stats
		if (GetOwnerRole() == ROLE_Authority)
		{

		}
	}
	return false;
}
void UAREquipmentComponent::DoAsyncChestChange()
{
	if (ChestMeshToLoad.ResolveObject() != nullptr)
	{
		UObject* NewChestMesh = ChestMeshToLoad.ResolveObject();

		//should change it to Withing, but for testing.. it will do just fine.

		AARCharacter* CharOwner = Cast<AARCharacter>(GetOwner());
		if (CharOwner)
		{
			CharOwner->ChestMesh->SetSkeletalMesh(Cast<USkeletalMesh>(NewChestMesh));
		}
	}
}

void UAREquipmentComponent::ChangeLegItem(FName ItemName)
{
	//this can be called by both client and server
	//server need to do it, to propagate mesh change
	//to other players.
	//and client
	UARItemDataAsset* ItemDataAsset = Cast<UARItemDataAsset>(StaticLoadObject(UARItemDataAsset::StaticClass(), NULL, *LegItemDataAssetPath, NULL, LOAD_None, NULL));

	if (ItemDataAsset && ItemDataAsset->Items.Num() > 0)
	{
		TArray<FStringAssetReference> ObjToLoad;
		FStreamableManager& Loader = UARSingleton::Get().AssetLoader;

		//do it this way until I figure out predicate...
		for (FARItemInfo& item : ItemDataAsset->Items)
		{
			if (item.ItemName == ItemName)
			{
				LegMeshToLoad = item.ItemMesh.ToStringReference();
				ObjToLoad.AddUnique(LegMeshToLoad);
				Loader.RequestAsyncLoad(ObjToLoad, FStreamableDelegate::CreateUObject(this, &UAREquipmentComponent::DoAsyncChestChange));
				//ChestItem = item.ItemName;
				return;
			}
		}
	}

	//this is only for server
	//because here we will be changing character stats
	if (GetOwnerRole() == ROLE_Authority)
	{

	}
	//return false;
}
void UAREquipmentComponent::DoAsyncLegChange()
{
	if (ChestMeshToLoad.ResolveObject() != nullptr)
	{
		UObject* NewChestMesh = ChestMeshToLoad.ResolveObject();

		//should change it to Withing, but for testing.. it will do just fine.

		AARCharacter* CharOwner = Cast<AARCharacter>(GetOwner());
		if (CharOwner)
		{
			CharOwner->ChestMesh->SetSkeletalMesh(Cast<USkeletalMesh>(NewChestMesh));
		}
	}
}

/* Weapon Handling **/

void UAREquipmentComponent::AddLeftHandWeapon(FName ItemName)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerAddLeftHandWeapon(ItemName);
	}
	else
	{
		for (FInventorySlot& weapon : TargetController->InventorySmall)
		{
			if (weapon.ItemID == ItemName)
			{
				LeftHandWeapons.Add(weapon);
				return;
			}
		}
	}
}
void UAREquipmentComponent::ServerAddLeftHandWeapon_Implementation(FName ItemName)
{
	AddLeftHandWeapon(ItemName);
}
bool UAREquipmentComponent::ServerAddLeftHandWeapon_Validate(FName ItemName)
{
	return true;
}

void UAREquipmentComponent::SwapLeftWeapon()
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerSwapLeftWeapon();
	}
	else
	{
		for (FInventorySlot& weapon : TargetController->LeftHandWeapons)
		{
			if (!weapon.ItemID.IsNone() && weapon.ItemID != ActiveLeftHandWeaponStruct.ItemID)
			{
				ActiveLeftHandWeaponStruct = weapon;
				SetLeftWeapon(ActiveLeftHandWeaponStruct, ActiveLeftHandWeapon);
				return;
			}
		}
	}
}
void UAREquipmentComponent::ServerSwapLeftWeapon_Implementation()
{
	SwapLeftWeapon();
}
bool UAREquipmentComponent::ServerSwapLeftWeapon_Validate()
{
	return true;
}

void UAREquipmentComponent::SetLeftWeapon(FInventorySlot Weapon, class AARWeapon* PrevWeapon)
{
	if (PrevWeapon)
	{
		PrevWeapon->Destroy();
	}

	FString usless;
	FARItemData* data = WeaponItemDataTable->FindRow<FARItemData>(Weapon.ItemID, usless);

	if (data)
	{
		UBlueprint* gen = LoadObject<UBlueprint>(NULL, *data->ItemBlueprint.ToStringReference().ToString(), NULL, LOAD_None, NULL);
		if (!gen)
			return;

		AARCharacter* MyChar = Cast<AARCharacter>(GetOwner());
		//AARCharacter* MyChar = Cast<AARCharacter>(GetOuterAARPlayerController()->GetPawn());
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.bNoCollisionFail = true;
		SpawnInfo.Owner = MyChar;
		AARWeapon* weaponBase = GetWorld()->SpawnActor<AARWeapon>(gen->GeneratedClass, SpawnInfo);
		weaponBase->SetOwner(MyChar);
		weaponBase->Instigator = MyChar;
		weaponBase->WeaponOwner = MyChar;
		ActiveLeftHandWeapon = weaponBase;
		SetAttachWeapon(ActiveLeftHandWeapon, LeftWeaponSocket);
	}
}

void UAREquipmentComponent::ServerSwapRightWeapon_Implementation()
{
	SwapRightWeapon();
}
bool UAREquipmentComponent::ServerSwapRightWeapon_Validate()
{
	return true;
}
void UAREquipmentComponent::SwapRightWeapon()
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerSwapRightWeapon();
	}
	else
	{
		for (FInventorySlot& weapon : TargetController->RightHandWeapons)
		{
			if (!weapon.ItemID.IsNone() && weapon.ItemID != ActiveRightHandWeaponStruct.ItemID)
			{
				ActiveRightHandWeaponStruct = weapon;
				SetRightWeapon(ActiveRightHandWeaponStruct, ActiveRightHandWeapon);
				return;
			}
		}
	}
}
void UAREquipmentComponent::SetRightWeapon(FInventorySlot Weapon, class AARWeapon* PrevWeapon)
{
	if (PrevWeapon)
	{
		PrevWeapon->Destroy();
	}

	FString usless;
	FARItemData* data = WeaponItemDataTable->FindRow<FARItemData>(Weapon.ItemID, usless);

	if (data)
	{
		UBlueprint* gen = LoadObject<UBlueprint>(NULL, *data->ItemBlueprint.ToStringReference().ToString(), NULL, LOAD_None, NULL);
		if (!gen)
			return;

		AARCharacter* MyChar = Cast<AARCharacter>(GetOwner());
		//AARCharacter* MyChar = Cast<AARCharacter>(GetOuterAARPlayerController()->GetPawn());
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.bNoCollisionFail = true;
		SpawnInfo.Owner = MyChar;
		AARWeapon* weaponBase = GetWorld()->SpawnActor<AARWeapon>(gen->GeneratedClass, SpawnInfo);
		weaponBase->SetOwner(MyChar);
		weaponBase->Instigator = MyChar;
		weaponBase->WeaponOwner = MyChar;
		ActiveRightHandWeapon = weaponBase;
		SetAttachWeapon(ActiveRightHandWeapon, RightWeaponSocket);
	}
}