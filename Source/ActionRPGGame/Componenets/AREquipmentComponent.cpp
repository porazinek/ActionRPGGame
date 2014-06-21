// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "ActionRPGGame.h"

#include "../Types/AREnumTypes.h"
#include "../Items/ARItem.h"
#include "../Items/ARWeapon.h"
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
}

void UAREquipmentComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	ItemDataAssetPath = "/Game/Blueprints/Items_DataAsset.Items_DataAsset";
	WeaponItemDataAssetPath = "/Game/Blueprints/Data/WeaponData.WeaponData";
}
void UAREquipmentComponent::BeginDestroy()
{
	Super::BeginDestroy();
	//InventorySlate.Reset();
}
void UAREquipmentComponent::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAREquipmentComponent, MainHand);
	DOREPLIFETIME(UAREquipmentComponent, OffHand);
	
	DOREPLIFETIME(UAREquipmentComponent, TargetCharacter);
	DOREPLIFETIME(UAREquipmentComponent, TargetController);

	//DOREPLIFETIME(UAREquipmentComponent, IsInventoryChanged);

	DOREPLIFETIME_CONDITION(UAREquipmentComponent, Inventory, COND_OwnerOnly);
	//DOREPLIFETIME(UAREquipmentComponent, InventoryData);

	DOREPLIFETIME(UAREquipmentComponent, ActiveLeftHandWeapon);


	DOREPLIFETIME(UAREquipmentComponent, ChestItem);
}
void UAREquipmentComponent::OnRep_MainHand()
{
	SetAttachWeapon(ActiveLeftHandWeapon, LeftWeaponSocket);
	//ClientEquipWeapon(MainHand, MainHandSocket);
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
	IsInventoryChanged = true;
}

void UAREquipmentComponent::PickupItem()
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerPickupItem();
	}
	else
	{
		FHitResult HitResult = UARTraceStatics::GetHitResult(1000.0f, "PickItem", TargetCharacter, false, false);

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
void UAREquipmentComponent::AddItemToInventory(FName ItemName)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		//add item on server
		ServerAddItemToInventory(ItemName);

		//add item on client
		//comsetics. Server have authrative list of items.
		if (TargetController)
		{
			if (TargetController->Inventory.Num() > 0)
			{
				float ten = 10;
			}
		}
	}
	else
	{
		if (TargetController)
		{
			UARItemDataAsset* ItemDataAsset = Cast<UARItemDataAsset>(StaticLoadObject(UARItemDataAsset::StaticClass(), NULL, *ItemDataAssetPath, NULL, LOAD_None, NULL));

			if (ItemDataAsset && ItemDataAsset->Items.Num() > 0)
			{
				for (FARItemInfo& item : ItemDataAsset->Items)
				{
					if (item.ItemName == ItemName)
					{
						//FInventorySlot
						TargetController->Inventory.Add(item);
						break;
					}
				}
			}
		}
	}
}
void UAREquipmentComponent::ServerAddItemToInventory_Implementation(FName ItemName)
{
	if (TargetController)
	{
		UARItemDataAsset* ItemDataAsset = Cast<UARItemDataAsset>(StaticLoadObject(UARItemDataAsset::StaticClass(), NULL, *ItemDataAssetPath, NULL, LOAD_None, NULL));

		if (ItemDataAsset && ItemDataAsset->Items.Num() > 0)
		{
			for (FARItemInfo& item : ItemDataAsset->Items)
			{
				if (item.ItemName == ItemName)
				{
					FInventorySlot invSlot;
					invSlot.ItemName = item.ItemName;
					invSlot.ItemSlot = item.ItemSlot;
					TargetController->InventorySmall.Add(invSlot);
					TargetController->AddItemToInventory(item);
					break;
				}
			}
		}
	}
}
bool UAREquipmentComponent::ServerAddItemToInventory_Validate(FName ItemName)
{
	return true;
}
TArray<FARItemInfo> UAREquipmentComponent::GetItems()
{
	return Inventory;
}

void UAREquipmentComponent::EquipWeapon(TSubclassOf<class AARWeapon> Weapon, FName SocketName, FName ItemName)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		if (Weapon)
		{
			ServerEquipWeapon(Weapon, SocketName, ItemName);
		}
	}
	else
	{
		if (Weapon)
		{

			UARItemDataAsset* ItemDataAsset = Cast<UARItemDataAsset>(StaticLoadObject(UARItemDataAsset::StaticClass(), NULL, *WeaponItemDataAssetPath, NULL, LOAD_None, NULL));

			if (ItemDataAsset && ItemDataAsset->Items.Num() > 0)
			{
				TArray<FStringAssetReference> ObjToLoad;
				FStreamableManager& Loader = UARSingleton::Get().AssetLoader;

				//do it this way until I figure out predicate...
				for (FARItemInfo& item : ItemDataAsset->Items)
				{
					//this probably need some rework.
					if (item.ItemName == ItemName)
					{
						AARCharacter* MyChar = Cast<AARCharacter>(GetOwner());
						//AARCharacter* MyChar = Cast<AARCharacter>(GetOuterAARPlayerController()->GetPawn());
						FActorSpawnParameters SpawnInfo;
						SpawnInfo.bNoCollisionFail = true;
						SpawnInfo.Owner = MyChar;
						AARWeapon* weaponBase = GetWorld()->SpawnActor<AARWeapon>(item.ItemType, SpawnInfo);
						weaponBase->SetOwner(MyChar);
						weaponBase->Instigator = MyChar;
						weaponBase->WeaponOwner = MyChar;
						MainHand = weaponBase;
						OffHand = weaponBase;
						SetAttachWeapon(weaponBase, item.AttachSocket);
						return;
					}
				}
				//ChestMeshToLoad = ItemDataAsset->Items.FindByPredicate
			}

			//AARCharacter* MyChar = Cast<AARCharacter>(GetOwner());
			////AARCharacter* MyChar = Cast<AARCharacter>(GetOuterAARPlayerController()->GetPawn());
			//FActorSpawnParameters SpawnInfo;
			//SpawnInfo.bNoCollisionFail = true;
			//SpawnInfo.Owner = MyChar;
			//AARWeapon* weaponBase = GetWorld()->SpawnActor<AARWeapon>(Weapon, SpawnInfo);
			//weaponBase->SetOwner(MyChar);
			//weaponBase->Instigator = MyChar;
			//weaponBase->WeaponOwner = MyChar;
			//MainHand = weaponBase;
			//OffHand = weaponBase;
			//SetAttachWeapon(weaponBase, SocketName);
		}
	}

}

void UAREquipmentComponent::ServerEquipWeapon_Implementation(TSubclassOf<class AARWeapon> Weapon, FName SocketName, FName ItemName)
{
	//we are on server now, so we just call function that brought us here again.
	EquipWeapon(Weapon, SocketName, ItemName);
}

bool UAREquipmentComponent::ServerEquipWeapon_Validate(TSubclassOf<class AARWeapon> Weapon, FName SocketName, FName ItemName)
{
	return true;
}

void UAREquipmentComponent::SetAttachWeapon(class AARWeapon* Weapon, FName SocketName)
{
	if (Weapon)
	{
		AARCharacter* MyChar = Cast<AARCharacter>(GetOwner());
		//AARCharacter* MyChar = Cast<AARCharacter>(GetOuterAARPlayerController()->GetPawn());
		if (MyChar)
		{
			Weapon->WeaponMesh->AttachTo(MyChar->Mesh, SocketName);
			Weapon->WeaponMesh->SetHiddenInGame(false);
		}
		Weapon->WeaponOwner = MyChar;
		//Weapon->AttachWeapon();
	}
}

void UAREquipmentComponent::ServerChangeChestItem_Implementation(FName ItemName)
{
	ChangeChestItem(ItemName);
}

bool UAREquipmentComponent::ServerChangeChestItem_Validate(FName ItemName)
{
	return true;
}

void UAREquipmentComponent::ChangeChestItem(FName ItemName)
{
	//this can be called by both client and server
	//server need to do it, to propagate mesh change
	//to other players.
	//and client
	UARItemDataAsset* ItemDataAsset = Cast<UARItemDataAsset>(StaticLoadObject(UARItemDataAsset::StaticClass(), NULL, *ItemDataAssetPath, NULL, LOAD_None, NULL));

	if (ItemDataAsset && ItemDataAsset->Items.Num() > 0)
	{
		TArray<FStringAssetReference> ObjToLoad;
		FStreamableManager& Loader = UARSingleton::Get().AssetLoader;

		//do it this way until I figure out predicate...
		for (FARItemInfo& item : ItemDataAsset->Items)
		{
			if (item.ItemName == ItemName)
			{
				ChestMeshToLoad = item.ItemMesh.ToStringReference();
				ObjToLoad.AddUnique(ChestMeshToLoad);
				Loader.RequestAsyncLoad(ObjToLoad, FStreamableDelegate::CreateUObject(this, &UAREquipmentComponent::DoAsyncChestChange));
				ChestItem = item.ItemName;
				return;
			}
		}
		//ChestMeshToLoad = ItemDataAsset->Items.FindByPredicate
	}

	//this is only for server
	//because here we will be changing character stats
	if (GetOwnerRole() == ROLE_Authority)
	{

	}
	//return false;
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

void UAREquipmentComponent::ChangeItemValid(FName ItemName)
{
	for (FARItemInfo& item : TargetController->Inventory)
	{
		if (item.ItemName == ItemName)
		{
			switch (item.ItemSlot)
			{
			case EItemSlot::Item_Weapon:
				EquipWeapon(NULL, item.AttachSocket, ItemName);
				break;
			case EItemSlot::Item_Chest:
				ChangeChestItem(item.ItemName);
				break;
			default:
				return;
			}
		}
	}
}

void UAREquipmentComponent::ServerChangeItem_Implementation(FName ItemName)
{
	ChangeItemValid(ItemName);
}
bool UAREquipmentComponent::ServerChangeItem_Validate(FName ItemName)
{
	return true;
}

void UAREquipmentComponent::ChangeItem(FName ItemName)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerChangeItem(ItemName);
	}
	else
	{
		ChangeItemValid(ItemName);
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
		for (FARItemInfo& weapon : TargetController->Inventory)
		{
			if (weapon.ItemName == ItemName)
			{
				LeftHandWeaponsShared.Add(weapon);
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
		const int32 CurrentWeaponIndex = LeftHandWeaponsShared.IndexOfByKey(ActiveLeftHandWeaponStruct);
		FARItemInfo weapon = LeftHandWeaponsShared[(CurrentWeaponIndex + 1) % LeftHandWeaponsShared.Num()];
		EquipLeftWeapon(weapon);
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

void UAREquipmentComponent::EquipLeftWeapon(FARItemInfo Weapon)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerEquipLeftWeapon(Weapon);
	}
	else
	{
		SetLeftWeapon(Weapon);
	}
}

void UAREquipmentComponent::ServerEquipLeftWeapon_Implementation(FARItemInfo Weapon)
{
	EquipLeftWeapon(Weapon);
}
bool UAREquipmentComponent::ServerEquipLeftWeapon_Validate(FARItemInfo Weapon)
{
	return true;
}

void UAREquipmentComponent::SetLeftWeapon(FARItemInfo Weapon)
{
	AARCharacter* MyChar = Cast<AARCharacter>(GetOwner());
	//AARCharacter* MyChar = Cast<AARCharacter>(GetOuterAARPlayerController()->GetPawn());
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoCollisionFail = true;
	SpawnInfo.Owner = MyChar;
	AARWeapon* weaponBase = GetWorld()->SpawnActor<AARWeapon>(Weapon.ItemType, SpawnInfo);
	weaponBase->SetOwner(MyChar);
	weaponBase->Instigator = MyChar;
	weaponBase->WeaponOwner = MyChar;
	ActiveLeftHandWeapon = weaponBase;
	SetAttachWeapon(ActiveLeftHandWeapon, LeftWeaponSocket);
}