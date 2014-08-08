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

#include "../Items/ARInventoryComponent.h"

#include "Net/UnrealNetwork.h"

#include "AREquipmentComponent.h"



UAREquipmentComponent::UAREquipmentComponent(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	bReplicates = true;
	bWantsInitializeComponent = true;
	bAutoRegister = true;
	/*
		I should do some default initialization for these pointer at startup.
		Otherwise everything is messed up.
		*/
	ActiveRightHandWeapon = nullptr;
	ActiveLeftHandWeapon = nullptr;
	LeftHandWeaponsUpdated = false;
	RightHandWeaponsUpdated = false;
}

void UAREquipmentComponent::InitializeComponent()
{
	Super::InitializeComponent();
	//bReplicates = true;
	if (GetOwnerRole() == ROLE_Authority)
	{
		if (ActiveLeftHandWeapon)
		{

		}
		if (!ActiveRightHandWeapon)
		{
			AARCharacter* MyChar = Cast<AARCharacter>(GetOwner());
			//AARCharacter* MyChar = Cast<AARCharacter>(GetOuterAARPlayerController()->GetPawn());
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.bNoCollisionFail = true;
			SpawnInfo.Owner = MyChar;
			ActiveRightHandWeapon = GetWorld()->SpawnActor<AARWeapon>(AARWeapon::StaticClass(), SpawnInfo);
			if (ActiveRightHandWeapon)
			{
				//ActiveRightHandWeapon->ItemName = Weapon.ItemID;
				ActiveRightHandWeapon->SetOwner(MyChar);
				ActiveRightHandWeapon->Instigator = MyChar;
				ActiveRightHandWeapon->WeaponOwner = MyChar;
				ActiveRightHandWeapon->OwningController = TargetController;
			}
		}
		for (int32 i = 0; i < 4; i++)
		{
			FInventorySlot in;
			in.ItemID = NAME_None;
			in.SlotID = i;
			in.EEquipmentSlot = EEquipmentSlot::Item_LeftHandOne;
			in.ItemSlot = EItemSlot::Item_Weapon;
			in.IsAttached = false;
			LeftHandWeapons.Add(in);
		}

		for (int32 i = 0; i < 4; i++)
		{
			FInventorySlot in;
			in.ItemID = NAME_None;
			in.SlotID = i;
			in.EEquipmentSlot = EEquipmentSlot::Item_RightHandOne;
			in.ItemSlot = EItemSlot::Item_Weapon;
			in.IsAttached = false;
			RightHandWeapons.Add(in);
		}

	}
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

	DOREPLIFETIME_CONDITION(UAREquipmentComponent, Inventory, COND_OwnerOnly);

	DOREPLIFETIME(UAREquipmentComponent, ActiveLeftHandWeapon);
	DOREPLIFETIME(UAREquipmentComponent, ActiveRightHandWeapon);

	DOREPLIFETIME(UAREquipmentComponent, ChestItem);
	/*
		This shit is very heavy, but... For 8 players coop, who cares ?

		*/
	DOREPLIFETIME(UAREquipmentComponent, LeftHandWeapons);
	DOREPLIFETIME(UAREquipmentComponent, RightHandWeapons);
}
void UAREquipmentComponent::OnRep_LeftHandWeapons()
{
	LeftHandWeaponsUpdated = true;
	//AttacheSheathedWeapon(LeftHandWeapons, WeaponSockets, 0);
}
void UAREquipmentComponent::OnRep_RightHandWeapons()
{
	RightHandWeaponsUpdated = true;
	//AttacheSheathedWeapon(RightHandWeapons, WeaponSockets, 1);
}

void UAREquipmentComponent::MulticastAttacheSheathedWeapon_Implementation(FInventorySlot WeaponIn, int32 HandIn)
{
	FString usless = "";

	FARItemData* data = WeaponItemDataTable->FindRow<FARItemData>(WeaponIn.ItemID, usless);

	if (data)
	{
		UBlueprint* gen = LoadObject<UBlueprint>(NULL, *data->ItemBlueprint.ToStringReference().ToString(), NULL, LOAD_None, NULL);
		if (!gen)
			return;

		FActorSpawnParameters SpawnInfo;
		SpawnInfo.bNoCollisionFail = true;
		SpawnInfo.Owner = TargetCharacter;
		AARWeapon* weaponBase = GetWorld()->SpawnActor<AARWeapon>(gen->GeneratedClass, SpawnInfo);
		weaponBase->ItemName = WeaponIn.ItemID;
		weaponBase->ItemID = WeaponIn.ItemID;
		weaponBase->SetOwner(TargetCharacter);
		weaponBase->Instigator = TargetCharacter;
		weaponBase->WeaponOwner = TargetCharacter;
		if (HandIn == 0)
			weaponBase->WeaponHand = EWeaponHand::WeaponLeft;
		else if (HandIn == 1)
			weaponBase->WeaponHand = EWeaponHand::WeaponRight;
		/*
		I definetly need more nested if's here!
		*/
		for (FARAttachmentSocket& socket : WeaponSockets)
		{
			if (weaponBase->AttachmentType == socket.AttachType)
			{
				if (socket.IsSlotAvailable)
				{
					if (socket.SocketSide != HandIn)
					{
						socket.IsSlotAvailable = false;
						socket.LastItemID = WeaponIn.ItemID;
						EquipedWeapons.Add(weaponBase);
						//WeaponIn.IsAttached = true;
						SetSeathedWeapon(weaponBase, socket.SocketName);
						weaponBase->LastAttachmentSocket = socket.SocketName;
					}
				}
			}
		}



	}
}

/*
	Need to implement weapon attaching to character when adding to equipment list.
	*/
void UAREquipmentComponent::AddWeapon(FInventorySlot Weapon, int32 SlotID, int32 Hand)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerAddWeapon(Weapon, SlotID, Hand);
	}
	else
	{
		if (Hand == 0)
		{
			for (FInventorySlot& weapon : LeftHandWeapons)
			{
				if (weapon.SlotID == SlotID && weapon.ItemID != NAME_None)
				{
					FInventorySlot oldItemTemp = weapon;
					weapon.ItemID = Weapon.ItemID;
					weapon.ItemSlot = Weapon.ItemSlot;
					weapon.EEquipmentSlot = Weapon.EEquipmentSlot;
					weapon.IsAttached = true;
					for (FInventorySlot& oldItem : Inventory->Inventory)
					{
						if (weapon.SlotID == oldItem.SlotID)
						{
							oldItem.ItemID = oldItemTemp.ItemID;
							oldItem.ItemSlot = oldItemTemp.ItemSlot;
							oldItem.EEquipmentSlot = oldItemTemp.EEquipmentSlot;
							//return;
						}
					}
					MulticastAttacheSheathedWeapon(Weapon, Hand);
					LeftHandWeaponsUpdated = true;
					//OnRep_InventoryChanged();
					return;
				}
				if (weapon.ItemID.IsNone() && weapon.SlotID == SlotID)
				{
					weapon.ItemID = Weapon.ItemID;
					weapon.ItemSlot = Weapon.ItemSlot;
					weapon.EEquipmentSlot = Weapon.EEquipmentSlot;
					weapon.IsAttached = true;
					MulticastAttacheSheathedWeapon(Weapon, Hand);
					LeftHandWeaponsUpdated = true;
					return;
				}
			}
		}
		else if (Hand == 1)
		{
			for (FInventorySlot& weapon : RightHandWeapons)
			{
				if (weapon.SlotID == SlotID && weapon.ItemID != NAME_None)
				{
					FInventorySlot oldItemTemp = weapon;
					weapon.ItemID = Weapon.ItemID;
					weapon.ItemSlot = Weapon.ItemSlot;
					weapon.EEquipmentSlot = Weapon.EEquipmentSlot;
					for (FInventorySlot& oldItem : Inventory->Inventory)
					{
						if (weapon.SlotID == oldItem.SlotID)
						{
							oldItem.ItemID = oldItemTemp.ItemID;
							oldItem.ItemSlot = oldItemTemp.ItemSlot;
							oldItem.EEquipmentSlot = oldItemTemp.EEquipmentSlot;
							//return;
						}
					}
					MulticastAttacheSheathedWeapon(Weapon, Hand);
					RightHandWeaponsUpdated = true;
					//OnRep_InventoryChanged();
					return;
				}
				if (weapon.ItemID.IsNone() && weapon.SlotID == SlotID)
				{
					weapon.ItemID = Weapon.ItemID;
					weapon.ItemSlot = Weapon.ItemSlot;
					weapon.EEquipmentSlot = Weapon.EEquipmentSlot;
					MulticastAttacheSheathedWeapon(Weapon, Hand);
					RightHandWeaponsUpdated = true;
					return;
				}
			}
		}
	}
}
void UAREquipmentComponent::ServerAddWeapon_Implementation(FInventorySlot Weapon, int32 SlotID, int32 Hand)
{
	AddWeapon(Weapon, SlotID, Hand);
}
bool UAREquipmentComponent::ServerAddWeapon_Validate(FInventorySlot Weapon, int32 SlotID, int32 Hand)
{
	return true;
}
/*
	There are two ways of doing it:
	1. We spawn attached weapon for each client on replication.
	2. We spawn attached weapon on server, and then replicate list of spawned weapons back to clients.

	1. When equiping weapon, we destroy attached weapon on each client, and immidietly spawn new
	one in hand.
	2. We don't destroy anything, we just detach weapon from attachment point,
	attach it to hand, and make it active.

	First option is more replication friendly, as we just replicate struct with fairly simple properties.
	Second option could be more accurate, and we wouldn't need to spawn/destroy actor every time.
	*/
/*
	Left hand weapons - right attachment slots.
	Right Hand weapons - left attachment slots.
	*/
void UAREquipmentComponent::AttacheSheathedWeapon(TArray<FInventorySlot> WeaponsIn, TArray<FARAttachmentSocket> WeaponSocketsIn, int32 HandIn)
{
	FString usless = "";

	for (FInventorySlot& Weapon : WeaponsIn)
	{
		//if (Weapon.IsAttached)
		//{
		FARItemData* data = WeaponItemDataTable->FindRow<FARItemData>(Weapon.ItemID, usless);

		if (data)
		{
			UBlueprint* gen = LoadObject<UBlueprint>(NULL, *data->ItemBlueprint.ToStringReference().ToString(), NULL, LOAD_None, NULL);
			if (!gen)
				return;

			FActorSpawnParameters SpawnInfo;
			SpawnInfo.bNoCollisionFail = true;
			SpawnInfo.Owner = TargetCharacter;
			AARWeapon* weaponBase = GetWorld()->SpawnActor<AARWeapon>(gen->GeneratedClass, SpawnInfo);
			weaponBase->ItemName = Weapon.ItemID;
			weaponBase->ItemID = Weapon.ItemID;
			weaponBase->SetOwner(TargetCharacter);
			weaponBase->Instigator = TargetCharacter;
			weaponBase->WeaponOwner = TargetCharacter;
			if (HandIn == 0)
				weaponBase->WeaponHand = EWeaponHand::WeaponLeft;
			else if (HandIn == 1)
				weaponBase->WeaponHand = EWeaponHand::WeaponRight;
			/*
				I definetly need more nested if's here!
				*/
			for (FARAttachmentSocket& socket : WeaponSockets)
			{
				if (weaponBase->AttachmentType == socket.AttachType)
				{
					if (socket.IsSlotAvailable)
					{
						if (socket.SocketSide != HandIn)
						{
							socket.IsSlotAvailable = false;
							socket.LastItemID = Weapon.ItemID;
							EquipedWeapons.Add(weaponBase);
							Weapon.IsAttached = true;
							SetSeathedWeapon(weaponBase, socket.SocketName);
							weaponBase->LastAttachmentSocket = socket.SocketName;
						}
					}
				}
			}



		}


		//}
	}
}
void UAREquipmentComponent::AttachSheathhWeaponOnSwap_Implementation(class AARWeapon* LastWeapon, const TArray<FARAttachmentSocket>& WeaponSocketsIn, int32 HandIn)
{
	FString usless = "";

	//if (!WeaponIn.IsAttached)
	//{
	FARItemData* data = WeaponItemDataTable->FindRow<FARItemData>(LastWeapon->ItemID, usless);

	if (data)
	{
		UBlueprint* gen = LoadObject<UBlueprint>(NULL, *data->ItemBlueprint.ToStringReference().ToString(), NULL, LOAD_None, NULL);
		if (!gen)
			return;

		FActorSpawnParameters SpawnInfo;
		SpawnInfo.bNoCollisionFail = true;
		SpawnInfo.Owner = TargetCharacter;
		AARWeapon* weaponBase = GetWorld()->SpawnActor<AARWeapon>(gen->GeneratedClass, SpawnInfo);
		weaponBase->ItemName = LastWeapon->ItemID;
		weaponBase->ItemID = LastWeapon->ItemID;
		weaponBase->SetOwner(TargetCharacter);
		weaponBase->Instigator = TargetCharacter;
		weaponBase->WeaponOwner = TargetCharacter;
		weaponBase->WeaponHand = EWeaponHand::WeaponLeft;

		for (FARAttachmentSocket& socket : WeaponSockets)
		{
			if (weaponBase->AttachmentType == socket.AttachType)
			{
				if (socket.IsSlotAvailable)
				{
					if (socket.SocketSide != HandIn)
					{
						socket.IsSlotAvailable = false;
						socket.LastItemID = LastWeapon->ItemID;
						EquipedWeapons.Add(weaponBase);
						//Weapon.IsAttached = true;
						SetSeathedWeapon(weaponBase, socket.SocketName);
						weaponBase->LastAttachmentSocket = socket.SocketName;
					}
				}
			}
		}

	}
	//}
}

void UAREquipmentComponent::DetachSheathedWeapon(AARWeapon* WeaponToDetach)
{
	for (AARWeapon* weap : EquipedWeapons)
	{
		if (weap->ItemID == WeaponToDetach->ItemID)
		{
			//for (FInventorySlot& weapInfo : )
			//We need to clear socket info from RightHandWeapons and LeftHandWeapons

			for (FARAttachmentSocket& socket : WeaponSockets)
			{
				if (socket.SocketName == weap->LastAttachmentSocket)
				{
					socket.IsSlotAvailable = true;
				}
			}

			EquipedWeapons.Remove(weap);
			weap->Destroy();
			return;
		}
	}
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
		Weapon->OnWeaponActive();
	}
}

void UAREquipmentComponent::SetSeathedWeapon(class AARWeapon* WeaponIn, FName SocketNameIn)
{
	if (WeaponIn)
	{
		AARCharacter* MyChar = Cast<AARCharacter>(GetOwner());
		if (MyChar)
		{
			WeaponIn->WeaponMesh->AttachTo(TargetCharacter->Mesh, SocketNameIn);
			WeaponIn->WeaponMesh->SetHiddenInGame(false);
			WeaponIn->WeaponMesh->SetSimulatePhysics(true);
			WeaponIn->WeaponMesh->WakeRigidBody();
			//WeaponIn->WeaponMesh->WakeAllRigidBodies();
			//WeaponIn->WeaponMesh->SetWorldRotation(FRotator(0, 0, 180), false);
		}
		WeaponIn->WeaponOwner = MyChar;
	}
}
void UAREquipmentComponent::MulticastDetachWeaponSlotSwap_Implementation(FName WeaponID, int32 HandIn)
{
	for (AARWeapon* weap : EquipedWeapons)
	{
		if (weap->ItemID == WeaponID &&
			weap->WeaponHand == HandIn)
		{
			for (FARAttachmentSocket& socket : WeaponSockets)
			{
				if (socket.SocketName == weap->LastAttachmentSocket)
				{
					socket.IsSlotAvailable = true;
				}
			}
			EquipedWeapons.Remove(weap);
			weap->Destroy();
		}
	}
}

bool UAREquipmentComponent::RemoveWeapon(FName Weapon, int32 SlotID, int32 Hand)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerRemoveWeapon(Weapon, SlotID, Hand);
	}
	else
	{
		if (Hand == 0)
		{
			for (FInventorySlot& item : LeftHandWeapons)
			{
				if (item.SlotID == SlotID && item.ItemID != NAME_None)
				{
					//we don't remove actually anything from array.
					//just change ID and slot types, to match an "empty" slot 
					// in inventory.
					item.ItemID = NAME_None;
					item.ItemSlot = EItemSlot::Item_Inventory;
					item.EEquipmentSlot = EEquipmentSlot::Item_LeftHandOne;
					LeftHandWeaponsUpdated = true;
					MulticastDetachWeaponSlotSwap(Weapon, Hand);
					return true;
				}
			}
		}
		else if (Hand == 1)
		{
			for (FInventorySlot& item : RightHandWeapons)
			{
				if (item.SlotID == SlotID && item.ItemID != NAME_None)
				{
					//we don't remove actually anything from array.
					//just change ID and slot types, to match an "empty" slot 
					// in inventory.
					item.ItemID = NAME_None;
					item.ItemSlot = EItemSlot::Item_Inventory;
					item.EEquipmentSlot = EEquipmentSlot::Item_RightHandOne;
					RightHandWeaponsUpdated = true;
					MulticastDetachWeaponSlotSwap(Weapon, Hand);
					return true;
				}
			}
		}
	}
	return false;
}
void UAREquipmentComponent::ServerRemoveWeapon_Implementation(FName Weapon, int32 SlotID, int32 Hand)
{
	RemoveWeapon(Weapon, SlotID, Hand);
}
bool UAREquipmentComponent::ServerRemoveWeapon_Validate(FName Weapon, int32 SlotID, int32 Hand)
{
	return true;
}

void UAREquipmentComponent::OnRep_AtiveLeftHandWeapon()
{
	//DetachSheathedWeapon(ActiveLeftHandWeapon);
	SetAttachWeapon(ActiveLeftHandWeapon, LeftWeaponSocket);
}
void UAREquipmentComponent::OnRep_ActiveRightHandWeapon()
{
	//DetachSheathedWeapon(ActiveRightHandWeapon);
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
				itemRemoved = Inventory->RemoveItemFromInventory(ItemIn.ItemID, ItemIn.SlotID);
				Inventory->AddItemToInventoryOnSlot(tempItem, OldItemSlotID);
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
					Inventory->RemoveItemFromInventory(ItemIn.ItemID, ItemIn.SlotID);
					break;
				}
			}
			break;
		}
		case EEquipmentSlot::Item_LeftHandOne:
		{
			AddWeapon(ItemIn, OldItemSlotID, 0);
		}
		case EEquipmentSlot::Item_RightHandOne:
		{
			AddWeapon(ItemIn, OldItemSlotID, 1);
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
	//UARItemDataAsset* ItemDataAsset = Cast<UARItemDataAsset>(StaticLoadObject(UARItemDataAsset::StaticClass(), NULL, *LegItemDataAssetPath, NULL, LOAD_None, NULL));

	//if (ItemDataAsset && ItemDataAsset->Items.Num() > 0)
	//{
	//	TArray<FStringAssetReference> ObjToLoad;
	//	FStreamableManager& Loader = UARSingleton::Get().AssetLoader;

	//	//do it this way until I figure out predicate...
	//	for (FARItemInfo& item : ItemDataAsset->Items)
	//	{
	//		if (item.ItemID == ItemName)
	//		{
	//			//LegMeshToLoad = item.ItemMesh.ToStringReference();
	//			ObjToLoad.AddUnique(LegMeshToLoad);
	//			Loader.RequestAsyncLoad(ObjToLoad, FStreamableDelegate::CreateUObject(this, &UAREquipmentComponent::DoAsyncChestChange));
	//			//ChestItem = item.ItemName;
	//			return;
	//		}
	//	}
	//}

	////this is only for server
	////because here we will be changing character stats
	//if (GetOwnerRole() == ROLE_Authority)
	//{

	//}
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
void UAREquipmentComponent::SwapWeapon(int32 Hand)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerSwapWeapon(Hand);
	}
	else
	{
		if (Hand == 0)
		{
			for (FInventorySlot& weapon : LeftHandWeapons)
			{
				if (!weapon.ItemID.IsNone() && weapon.ItemID != ActiveLeftHandWeaponStruct.ItemID)
				{
					weapon.IsAttached = false;

					ActiveLeftHandWeaponStruct = weapon;

					SetWeapon(ActiveLeftHandWeaponStruct, ActiveLeftHandWeapon, Hand);
					return;
				}
			}
		}
		else if (Hand == 1)
		{
			for (FInventorySlot& weapon : RightHandWeapons)
			{
				if (!weapon.ItemID.IsNone() && weapon.ItemID != ActiveRightHandWeaponStruct.ItemID)
				{
					ActiveRightHandWeaponStruct = weapon;
					SetWeapon(ActiveRightHandWeaponStruct, ActiveRightHandWeapon, Hand);
					return;
				}
			}
		}
	}
}
void UAREquipmentComponent::ServerSwapWeapon_Implementation(int32 Hand)
{
	SwapWeapon(Hand);
}
bool UAREquipmentComponent::ServerSwapWeapon_Validate(int32 Hand)
{
	return true;
}
void UAREquipmentComponent::SetWeapon(FInventorySlot Weapon, class AARWeapon* PrevWeapon, int32 Hand)
{
	if (Hand == 0)
	{
		if (PrevWeapon)
		{
			AttachSheathhWeaponOnSwap(PrevWeapon, WeaponSockets, Hand);
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
			weaponBase->ItemName = Weapon.ItemID;
			weaponBase->ItemID = Weapon.ItemID;
			weaponBase->SetOwner(MyChar);
			weaponBase->Instigator = MyChar;
			weaponBase->WeaponOwner = MyChar;
			weaponBase->WeaponHand = EWeaponHand::WeaponLeft;
			ActiveLeftHandWeapon = weaponBase;
			//ActiveLeftHandWeapon->OnWeaponActive();
			//OnRightWeaponActive.Broadcast(ActiveLeftHandWeapon);
			ActiveLeftHandWeapon->Initialize();
			MulticastDetachWeaponSlotSwap(Weapon.ItemID, Hand);
			SetAttachWeapon(ActiveLeftHandWeapon, LeftWeaponSocket);
		}
	}
	else if (Hand == 1)
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

			weaponBase->ItemName = Weapon.ItemID;
			weaponBase->ItemID = Weapon.ItemID;
			weaponBase->SetOwner(MyChar);
			weaponBase->Instigator = MyChar;
			weaponBase->WeaponOwner = MyChar;
			//weaponBase->OwningController = TargetController;
			weaponBase->WeaponHand = EWeaponHand::WeaponRight;
			ActiveRightHandWeapon = weaponBase;
			ActiveRightHandWeapon->Initialize();
			//ActiveLeftHandWeapon->OnWeaponActive();
			//OnRightWeaponActive.Broadcast(ActiveRightHandWeapon);
			MulticastDetachWeaponSlotSwap(Weapon.ItemID, Hand);
			SetAttachWeapon(ActiveRightHandWeapon, RightWeaponSocket);
		}
	}

}
void UAREquipmentComponent::UnEquipWeapon(FName ItemID, int32 Hand)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerUnEquipWeapon(ItemID, Hand);
	}
	else
	{
		if (Hand == 0)
		{
			/*
			In reality it should attach weapon do different socket.
			Each socket coresponds to single slot in PlayerController->LeftHandWeapons
			Bur for now it will suffice.
			*/
			if (ActiveLeftHandWeapon && ActiveLeftHandWeapon->ItemName == ItemID)
			{
				//we don't want to move item back to inventory.
				//we just need to reset pointer to weapon, and un attach mesh.
				ActiveLeftHandWeaponStruct.ItemID = NAME_None;
				ActiveLeftHandWeapon->SetActorHiddenInGame(true);
				ActiveLeftHandWeapon = nullptr;
			}
		}
		else if (Hand == 1)
		{
			if (ActiveRightHandWeapon && ActiveRightHandWeapon->ItemName == ItemID)
			{
				//we don't want to move item back to inventory.
				//we just need to reset pointer to weapon, and un attach mesh.
				ActiveRightHandWeaponStruct.ItemID = NAME_None;
				ActiveRightHandWeapon->SetActorHiddenInGame(true);
				ActiveRightHandWeapon = nullptr;
			}
		}
	}
}
void UAREquipmentComponent::ServerUnEquipWeapon_Implementation(FName ItemID, int32 Hand)
{
	UnEquipWeapon(ItemID, Hand);
}
bool UAREquipmentComponent::ServerUnEquipWeapon_Validate(FName ItemID, int32 Hand)
{
	return true;
}
