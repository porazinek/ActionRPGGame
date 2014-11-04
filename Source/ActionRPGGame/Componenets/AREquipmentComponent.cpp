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



UAREquipmentComponent::UAREquipmentComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
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
				//ActiveRightHandWeapon->ItemName = Weapon.ARItemID;
				ActiveRightHandWeapon->SetOwner(MyChar);
				ActiveRightHandWeapon->Instigator = MyChar;
				ActiveRightHandWeapon->ARCharacterOwner = MyChar;
				ActiveRightHandWeapon->ARPCOwner = ARPCOwner;
			}
		}
		for (int32 i = 0; i < 4; i++)
		{
			FARDragDropInfo in;
			in.ItemKey = NAME_None;
			in.ItemIndex = INDEX_NONE;
			in.SlotName = "LHand";
			in.SlotIndex = i;
			in.DragDropSlot = EDragDropSlot::LeftHand;
			in.ItemSlot = EItemSlot::Item_Weapon;
			in.IsAttached = false;
			LeftHandWeapons.Add(in);
		}

		for (int32 i = 0; i < 4; i++)
		{
			FARDragDropInfo in;
			in.ItemKey = NAME_None;
			in.ItemIndex = INDEX_NONE;
			in.SlotName = "RHand";
			in.SlotIndex = i;
			in.DragDropSlot = EDragDropSlot::RightHand;
			in.ItemSlot = EItemSlot::Item_Weapon;
			in.IsAttached = false;
			RightHandWeapons.Add(in);
		}


		FARDragDropInfo HeadItem;
		HeadItem.ItemKey = NAME_None;
		HeadItem.ItemIndex = INDEX_NONE;
		HeadItem.SlotName = "Head";
		HeadItem.SlotIndex = 0;
		HeadItem.DragDropSlot = EDragDropSlot::Head;
		HeadItem.ItemSlot = EItemSlot::Item_Head;
		HeadItem.IsAttached = false;
		EquipmentSlots.Add(HeadItem);

		FARDragDropInfo ShouldersItem;
		ShouldersItem.ItemKey = NAME_None;
		ShouldersItem.ItemIndex = INDEX_NONE;
		ShouldersItem.SlotName = "Shoulders";
		ShouldersItem.SlotIndex = 1;
		ShouldersItem.DragDropSlot = EDragDropSlot::Shoulders;
		ShouldersItem.ItemSlot = EItemSlot::Item_Shoulders;
		ShouldersItem.IsAttached = false;
		EquipmentSlots.Add(ShouldersItem);

		FARDragDropInfo ChestItem;
		ChestItem.ItemKey = NAME_None;
		ChestItem.ItemIndex = INDEX_NONE;
		ChestItem.SlotName = "Chest";
		ChestItem.SlotIndex = 2;
		ChestItem.DragDropSlot = EDragDropSlot::Chest;
		ChestItem.ItemSlot = EItemSlot::Item_Chest;
		ChestItem.IsAttached = false;
		EquipmentSlots.Add(ChestItem);

		FARDragDropInfo HandsItem;
		HandsItem.ItemKey = NAME_None;
		HandsItem.ItemIndex = INDEX_NONE;
		HandsItem.SlotName = "Hands";
		HandsItem.SlotIndex = 3;
		HandsItem.DragDropSlot = EDragDropSlot::Hands;
		HandsItem.ItemSlot = EItemSlot::Item_Hands;
		HandsItem.IsAttached = false;
		EquipmentSlots.Add(HandsItem);

		FARDragDropInfo LegsItem;
		LegsItem.ItemKey = NAME_None;
		LegsItem.ItemIndex = INDEX_NONE;
		LegsItem.SlotName = "Legs";
		LegsItem.SlotIndex = 4;
		LegsItem.DragDropSlot = EDragDropSlot::Legs;
		LegsItem.ItemSlot = EItemSlot::Item_Legs;
		LegsItem.IsAttached = false;
		EquipmentSlots.Add(LegsItem);

		FARDragDropInfo FootItem;
		FootItem.ItemKey = NAME_None;
		FootItem.ItemIndex = INDEX_NONE;
		FootItem.SlotName = "Foot";
		FootItem.SlotIndex = 5;
		FootItem.DragDropSlot = EDragDropSlot::Foot;
		FootItem.ItemSlot = EItemSlot::Item_Foot;
		FootItem.IsAttached = false;
		EquipmentSlots.Add(FootItem);
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

	DOREPLIFETIME(UAREquipmentComponent, ARCharacterOwner);
	DOREPLIFETIME(UAREquipmentComponent, ARPCOwner);

	DOREPLIFETIME_CONDITION(UAREquipmentComponent, Inventory, COND_OwnerOnly);

	DOREPLIFETIME(UAREquipmentComponent, ActiveLeftHandWeapon);
	DOREPLIFETIME(UAREquipmentComponent, ActiveRightHandWeapon);

	DOREPLIFETIME_CONDITION(UAREquipmentComponent, EquipmentSlots, COND_OwnerOnly);

	DOREPLIFETIME(UAREquipmentComponent, ChestItem);
	/*
		This shit is very heavy, but... For 8 players coop, who cares ?

		*/
	DOREPLIFETIME(UAREquipmentComponent, LeftHandWeapons);
	DOREPLIFETIME(UAREquipmentComponent, RightHandWeapons);
}


void UAREquipmentComponent::EquipItem(FARDragDropInfo ItemIn)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerEquipItem(ItemIn);
	}
	else
	{

	}
}
void UAREquipmentComponent::ServerEquipItem_Implementation(FARDragDropInfo ItemIn)
{
	EquipItem(ItemIn);
}
bool UAREquipmentComponent::ServerEquipItem_Validate(FARDragDropInfo ItemIn)
{
	return true;
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

void UAREquipmentComponent::MulticastAttacheSheathedWeapon_Implementation(FARDragDropInfo WeaponIn, int32 HandIn)
{
	//FARItemData* data = WeaponItemDataTable->FindRow<FARItemData>(WeaponIn.ARItemID, usless);
	FARItemInfo* data = TestItems->GetItemDataFromArrayPtr(WeaponIn.ItemIndex);
	if (data)
	{
		//UBlueprint* gen = LoadObject<UBlueprint>(NULL, *data->ItemBlueprint.ToStringReference().ToString(), NULL, LOAD_None, NULL);
		//if (!gen)
		//	return;

		FActorSpawnParameters SpawnInfo;
		SpawnInfo.bNoCollisionFail = true;
		SpawnInfo.Owner = ARCharacterOwner;
		AARWeapon* weaponBase;
		if (data->DefaultClass)
			weaponBase = GetWorld()->SpawnActor<AARWeapon>(data->DefaultClass, SpawnInfo);
		else
			weaponBase = GetWorld()->SpawnActor<AARWeapon>(AARWeapon::StaticClass(), SpawnInfo);

		weaponBase->ItemName = WeaponIn.ItemKey;
		weaponBase->ARItemID = WeaponIn.ItemKey;
		weaponBase->ItemIndex = WeaponIn.ItemIndex;
		weaponBase->SetOwner(ARCharacterOwner);
		weaponBase->Instigator = ARCharacterOwner;
		weaponBase->ARCharacterOwner = ARCharacterOwner;
		weaponBase->ItemInfo = data->ItemInfo;

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
						socket.LastItemID = WeaponIn.ItemKey;
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
void UAREquipmentComponent::AddWeapon(FARDragDropInfo Weapon, int32 SlotID, int32 Hand)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerAddWeapon(Weapon, SlotID, Hand);
	}
	else
	{
		if (Weapon.ItemSlot == EItemSlot::Item_Weapon
			|| Weapon.ItemSlot == EItemSlot::Item_TwoHanded)
		{

		}
		if (Hand == 0)
		{
			for (FARDragDropInfo& weapon : LeftHandWeapons)
			{
				if (weapon.SlotIndex == SlotID && weapon.ItemKey != NAME_None)
				{
					FARDragDropInfo oldItemTemp = weapon;
					weapon.ItemKey = Weapon.ItemKey;
					weapon.ItemIndex = Weapon.ItemIndex;
					weapon.ItemSlot = Weapon.ItemSlot;
					weapon.IsAttached = true;
					for (FARDragDropInfo& oldItem : Inventory->Inventory)
					{
						if (weapon.SlotIndex == oldItem.SlotIndex)
						{
							oldItem.ItemKey = oldItemTemp.ItemKey;
							oldItem.ItemIndex = oldItemTemp.ItemIndex;
							oldItem.ItemSlot = oldItemTemp.ItemSlot;
							//return;
						}
					}
					MulticastAttacheSheathedWeapon(Weapon, Hand);
					LeftHandWeaponsUpdated = true;
					//OnRep_InventoryChanged();
					return;
				}
				if (weapon.ItemKey.IsNone() && weapon.SlotIndex == SlotID)
				{
					weapon.ItemKey = Weapon.ItemKey;
					weapon.ItemIndex = Weapon.ItemIndex;
					weapon.ItemSlot = Weapon.ItemSlot;
					weapon.IsAttached = true;
					MulticastAttacheSheathedWeapon(Weapon, Hand);
					LeftHandWeaponsUpdated = true;
					return;
				}
			}
		}
		else if (Hand == 1)
		{
			for (FARDragDropInfo& weapon : RightHandWeapons)
			{
				if (weapon.SlotIndex == SlotID && weapon.ItemKey != NAME_None)
				{
					FARDragDropInfo oldItemTemp = weapon;
					weapon.ItemKey = Weapon.ItemKey;
					weapon.ItemIndex = Weapon.ItemIndex;
					weapon.ItemSlot = Weapon.ItemSlot;
				//	weapon.DragDropSlot = Weapon.DragDropSlot;
					for (FARDragDropInfo& oldItem : Inventory->Inventory)
					{
						if (weapon.SlotIndex == oldItem.SlotIndex)
						{
							oldItem.ItemKey = oldItemTemp.ItemKey;
							oldItem.ItemIndex = oldItemTemp.ItemIndex;
							oldItem.ItemSlot = oldItemTemp.ItemSlot;
					//		oldItem.DragDropSlot = oldItemTemp.DragDropSlot;
							//return;
						}
					}
					MulticastAttacheSheathedWeapon(Weapon, Hand);
					RightHandWeaponsUpdated = true;
					//OnRep_InventoryChanged();
					return;
				}
				if (weapon.ItemKey.IsNone() && weapon.SlotIndex == SlotID)
				{
					weapon.ItemKey = Weapon.ItemKey;
					weapon.ItemIndex = Weapon.ItemIndex;
					weapon.ItemSlot = Weapon.ItemSlot;
				//	weapon.DragDropSlot = Weapon.DragDropSlot;
					MulticastAttacheSheathedWeapon(Weapon, Hand);
					RightHandWeaponsUpdated = true;
					return;
				}
			}
		}
	}
}
void UAREquipmentComponent::ServerAddWeapon_Implementation(FARDragDropInfo Weapon, int32 SlotID, int32 Hand)
{
	AddWeapon(Weapon, SlotID, Hand);
}
bool UAREquipmentComponent::ServerAddWeapon_Validate(FARDragDropInfo Weapon, int32 SlotID, int32 Hand)
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
void UAREquipmentComponent::AttacheSheathedWeapon(TArray<FARDragDropInfo> WeaponsIn, TArray<FARAttachmentSocket> WeaponSocketsIn, int32 HandIn)
{
	FString usless = "";

	for (FARDragDropInfo& Weapon : WeaponsIn)
	{
		//if (Weapon.IsAttached)
		//{
		FARItemInfo* data = TestItems->GetItemDataFromArrayPtr(Weapon.ItemIndex);

		if (data)
		{
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.bNoCollisionFail = true;
			SpawnInfo.Owner = ARCharacterOwner;
			
			AARWeapon* weaponBase;
			if (data->DefaultClass)
				weaponBase = GetWorld()->SpawnActor<AARWeapon>(data->DefaultClass, SpawnInfo);
			else
				weaponBase = GetWorld()->SpawnActor<AARWeapon>(AARWeapon::StaticClass(), SpawnInfo);
			
			weaponBase->ItemName = Weapon.ItemKey;
			weaponBase->ARItemID = Weapon.ItemKey;
			weaponBase->ItemIndex = Weapon.ItemIndex;
			weaponBase->ItemInfo = data->ItemInfo;

			weaponBase->SetOwner(ARCharacterOwner);
			weaponBase->Instigator = ARCharacterOwner;
			weaponBase->ARCharacterOwner = ARCharacterOwner;
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
							socket.LastItemID = Weapon.ItemKey;
							EquipedWeapons.Add(weaponBase);
							Weapon.IsAttached = true;
							SetSeathedWeapon(weaponBase, socket.SocketName);
							weaponBase->LastAttachmentSocket = socket.SocketName;
						}
					}
				}
			}
		}
	}
}
void UAREquipmentComponent::AttachSheathhWeaponOnSwap_Implementation(class AARWeapon* LastWeapon, const TArray<FARAttachmentSocket>& WeaponSocketsIn, int32 HandIn)
{
	//FString usless = "";

	//if (!WeaponIn.IsAttached)
	//{
	//FARItemData* data = WeaponItemDataTable->FindRow<FARItemData>(LastWeapon->ARItemID, usless);
	FARItemInfo* data = TestItems->GetItemDataFromArrayPtr(LastWeapon->ItemIndex);
	if (data)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.bNoCollisionFail = true;
		SpawnInfo.Owner = ARCharacterOwner;
		
		AARWeapon* weaponBase;
		if (data->DefaultClass)
			weaponBase = GetWorld()->SpawnActor<AARWeapon>(data->DefaultClass, SpawnInfo);
		else
			weaponBase = GetWorld()->SpawnActor<AARWeapon>(AARWeapon::StaticClass(), SpawnInfo);
		
		weaponBase->ItemName = LastWeapon->ARItemID;
		weaponBase->ARItemID = LastWeapon->ARItemID;
		weaponBase->ItemIndex = LastWeapon->ItemIndex;
		weaponBase->SetOwner(ARCharacterOwner);
		weaponBase->Instigator = ARCharacterOwner;
		weaponBase->ARCharacterOwner = ARCharacterOwner;
		weaponBase->WeaponHand = EWeaponHand::WeaponLeft;
		weaponBase->ItemInfo = data->ItemInfo;


		for (FARAttachmentSocket& socket : WeaponSockets)
		{
			if (weaponBase->AttachmentType == socket.AttachType)
			{
				if (socket.IsSlotAvailable)
				{
					if (socket.SocketSide != HandIn)
					{
						socket.IsSlotAvailable = false;
						socket.LastItemID = LastWeapon->ARItemID;
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
/*
	Make it run on Server.
*/
void UAREquipmentComponent::DetachSheathedWeapon(AARWeapon* WeaponToDetach)
{
	for (AARWeapon* weap : EquipedWeapons)
	{
		if (weap->ARItemID == WeaponToDetach->ARItemID)
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
			Weapon->WeaponMesh->AttachTo(MyChar->GetMesh(), SocketName);
			Weapon->WeaponMesh->SetHiddenInGame(false);
		}
		Weapon->ARCharacterOwner = MyChar;
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
			WeaponIn->WeaponMesh->AttachTo(ARCharacterOwner->GetMesh(), SocketNameIn);
			WeaponIn->WeaponMesh->SetHiddenInGame(false);
			WeaponIn->WeaponMesh->SetSimulatePhysics(true);
			WeaponIn->WeaponMesh->WakeRigidBody();
			//WeaponIn->WeaponMesh->WakeAllRigidBodies();
			//WeaponIn->WeaponMesh->SetWorldRotation(FRotator(0, 0, 180), false);
		}
		WeaponIn->ARCharacterOwner = MyChar;
	}
}
void UAREquipmentComponent::MulticastDetachWeaponSlotSwap_Implementation(FName WeaponID, int32 HandIn)
{
	if (EquipedWeapons.Num() > 0)
	{
		for (AARWeapon* weap : EquipedWeapons)
		{
			if (weap->ARItemID == WeaponID &&
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
				return;
			}
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
			for (FARDragDropInfo& item : LeftHandWeapons)
			{
				if (item.SlotIndex == SlotID && item.ItemKey != NAME_None)
				{
					//we don't remove actually anything from array.
					//just change ID and slot types, to match an "empty" slot 
					// in inventory.
					item.ItemKey = NAME_None;
					item.ItemIndex = INDEX_NONE;
					item.ItemSlot = EItemSlot::Item_Inventory;
					item.DragDropSlot = EDragDropSlot::LeftHand;
					LeftHandWeaponsUpdated = true;
					MulticastDetachWeaponSlotSwap(Weapon, Hand);
					return true;
				}
			}
		}
		else if (Hand == 1)
		{
			for (FARDragDropInfo& item : RightHandWeapons)
			{
				if (item.SlotIndex == SlotID && item.ItemKey != NAME_None)
				{
					//we don't remove actually anything from array.
					//just change ID and slot types, to match an "empty" slot 
					// in inventory.
					item.ItemKey = NAME_None;
					item.ItemIndex = INDEX_NONE;
					item.ItemSlot = EItemSlot::Item_Inventory;
					item.DragDropSlot = EDragDropSlot::RightHand;
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
	//for (int32 ARItemIDx = 0; ARItemIDx < EquippedItems.Num(); ARItemIDx++)
	//{
	//	for (int32 i = 0; i < EquippedItems.Num() - 1; i++)
	//	{
	//		for (int32 attrIdx = 0; attrIdx < EquippedItems[ARItemIDx].Attributes.Num(); attrIdx++)
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
void UAREquipmentComponent::ServerChangeItem_Implementation(FARDragDropInfo ItemIn, int32 OldItemSlotID)
{
	ChangeItem(ItemIn, OldItemSlotID);
}
bool UAREquipmentComponent::ServerChangeItem_Validate(FARDragDropInfo ItemIn, int32 OldItemSlotID)
{
	return true;
}
void UAREquipmentComponent::ChangeItem(FARDragDropInfo ItemIn, int32 OldItemSlotID)
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
				FARDragDropInfo tempItem;
				tempItem.ItemKey = eqItem->ARItemID;
				tempItem.ItemSlot = eqItem->ItemSlotEquipped;
				//if there is, Unequip it. Before we proceed.
				UnEquipItem(ItemIn);
				itemRemoved = Inventory->RemoveItemFromInventory(ItemIn.ItemKey, ItemIn.SlotIndex);
				Inventory->AddItemToInventoryOnSlot(tempItem, OldItemSlotID);
			}
		}
		switch (ItemIn.DragDropSlot)
		{
		case EDragDropSlot::Chest:
		{
			if (ChangeChestItem(ItemIn))
			{
				if (!itemRemoved)
				{
					Inventory->RemoveItemFromInventory(ItemIn.ItemKey, ItemIn.SlotIndex);
					break;
				}
			}
			break;
		}
		case EDragDropSlot::LeftHand:
		{
			AddWeapon(ItemIn, OldItemSlotID, 0);
		}
		case EDragDropSlot::RightHand:
		{
			AddWeapon(ItemIn, OldItemSlotID, 1);
		}
		default:
			break;
		}
	}
}

void UAREquipmentComponent::UnEquipItem(FARDragDropInfo ItemIn)
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
void UAREquipmentComponent::ServerUnEquipItem_Implementation(FARDragDropInfo ItemIn)
{
	UnEquipItem(ItemIn);
}
bool UAREquipmentComponent::ServerUnEquipItem_Validate(FARDragDropInfo ItemIn)
{
	return true;
}

void UAREquipmentComponent::ServerChangeChestItem_Implementation(FARDragDropInfo ItemIn)
{
	ChangeChestItem(ItemIn);
}
bool UAREquipmentComponent::ServerChangeChestItem_Validate(FARDragDropInfo ItemIn)
{
	return true;
}
bool UAREquipmentComponent::ChangeChestItem(FARDragDropInfo ItemIn)
{
	FStreamableManager& Loader = UARSingleton::Get().AssetLoader;
	TArray<FStringAssetReference> ObjToLoad;

	//UBlueprint* gen = Cast<UBlueprint>(itemObj);
	//this can be called by both client and server
	//server need to do it, to propagate mesh change
	//to other players.
	//and client
	FString usless;
	FARItemData* data = ChestItemDataTable->FindRow<FARItemData>(ItemIn.ItemKey, usless);

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
		itemBase->ARItemID = ItemIn.ItemKey;
		itemBase->ItemSlotEquipped = ItemIn.ItemSlot;
		if (itemBase)
		{
			for (int32 ARItemIDx = 0; ARItemIDx < EquippedItems.Num(); ARItemIDx++)
			{
				if (EquippedItems[ARItemIDx]->IsA(itemBase->GetClass()))
				{
					EquippedItems[ARItemIDx]->Destroy();
					EquippedItems.RemoveAt(ARItemIDx);
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
	//		if (item.ARItemID == ItemName)
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
			for (FARDragDropInfo& weapon : LeftHandWeapons)
			{
				if (!weapon.ItemKey.IsNone() && weapon.ItemKey != ActiveLeftHandWeaponStruct.ItemKey)
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
			for (FARDragDropInfo& weapon : RightHandWeapons)
			{
				if (!weapon.ItemKey.IsNone() && weapon.ItemKey != ActiveRightHandWeaponStruct.ItemKey)
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
void UAREquipmentComponent::SetWeapon(FARDragDropInfo Weapon, class AARWeapon* PrevWeapon, int32 Hand)
{
	if (Hand == 0)
	{
		if (PrevWeapon)
		{
			AttachSheathhWeaponOnSwap(PrevWeapon, WeaponSockets, Hand);
			PrevWeapon->Destroy();
		}

		FARItemInfo* data = TestItems->GetItemDataFromArrayPtr(Weapon.ItemIndex);
		if (data)
		{
			AARCharacter* MyChar = Cast<AARCharacter>(GetOwner());
			//AARCharacter* MyChar = Cast<AARCharacter>(GetOuterAARPlayerController()->GetPawn());
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.bNoCollisionFail = true;
			SpawnInfo.Owner = MyChar;
			AARWeapon* weaponBase;
			if (data->DefaultClass)
				weaponBase = GetWorld()->SpawnActor<AARWeapon>(data->DefaultClass, SpawnInfo);
			else
				weaponBase = GetWorld()->SpawnActor<AARWeapon>(AARWeapon::StaticClass(), SpawnInfo);
			
			weaponBase->ItemName = Weapon.ItemKey;
			weaponBase->ARItemID = Weapon.ItemKey;
			weaponBase->ItemIndex = Weapon.ItemIndex;
			weaponBase->SetOwner(MyChar);
			weaponBase->Instigator = MyChar;
			weaponBase->ARCharacterOwner = MyChar;
			weaponBase->ARPCOwner = ARPCOwner;
			weaponBase->WeaponHand = EWeaponHand::WeaponLeft;

			weaponBase->ItemInfo = data->ItemInfo;

			ActiveLeftHandWeapon = weaponBase;
			//ActiveLeftHandWeapon->OnWeaponActive();
			//OnRightWeaponActive.Broadcast(ActiveLeftHandWeapon);
			ActiveLeftHandWeapon->Initialize();
			OnLeftWeaponActive.Broadcast(ActiveLeftHandWeapon);
			MulticastDetachWeaponSlotSwap(Weapon.ItemKey, Hand);
			SetAttachWeapon(ActiveLeftHandWeapon, LeftWeaponSocket);
		}
	}
	else if (Hand == 1)
	{
		if (PrevWeapon)
		{
			PrevWeapon->Destroy();
		}

		FARItemInfo* data = TestItems->GetItemDataFromArrayPtr(Weapon.ItemIndex);
		if (data)
		{
			AARCharacter* MyChar = Cast<AARCharacter>(GetOwner());
			//AARCharacter* MyChar = Cast<AARCharacter>(GetOuterAARPlayerController()->GetPawn());
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.bNoCollisionFail = true;
			SpawnInfo.Owner = MyChar;

			AARWeapon* weaponBase;
			if (data->DefaultClass)
				weaponBase = GetWorld()->SpawnActor<AARWeapon>(data->DefaultClass, SpawnInfo);
			else
				weaponBase = GetWorld()->SpawnActor<AARWeapon>(AARWeapon::StaticClass(), SpawnInfo);

			weaponBase->ItemName = Weapon.ItemKey;
			weaponBase->ARItemID = Weapon.ItemKey;
			weaponBase->ItemIndex = Weapon.ItemIndex;
			weaponBase->SetOwner(MyChar);
			weaponBase->Instigator = MyChar;
			weaponBase->ARCharacterOwner = MyChar;
			weaponBase->ARPCOwner = ARPCOwner;
			weaponBase->WeaponHand = EWeaponHand::WeaponRight;

			weaponBase->ItemInfo = data->ItemInfo;

			ActiveRightHandWeapon = weaponBase;
			ActiveRightHandWeapon->Initialize();
			OnRightWeaponActive.Broadcast(ActiveRightHandWeapon);
			//ActiveLeftHandWeapon->OnWeaponActive();
			//OnRightWeaponActive.Broadcast(ActiveRightHandWeapon);
			MulticastDetachWeaponSlotSwap(Weapon.ItemKey, Hand);
			SetAttachWeapon(ActiveRightHandWeapon, RightWeaponSocket);
		}
	}

}
void UAREquipmentComponent::UnEquipWeapon(FName ARItemID, int32 Hand)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerUnEquipWeapon(ARItemID, Hand);
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
			if (ActiveLeftHandWeapon && ActiveLeftHandWeapon->ItemName == ARItemID)
			{
				//we don't want to move item back to inventory.
				//we just need to reset pointer to weapon, and un attach mesh.
				ActiveLeftHandWeaponStruct.ItemKey = NAME_None;
				ActiveLeftHandWeaponStruct.ItemIndex = INDEX_NONE;
				ActiveLeftHandWeapon->SetActorHiddenInGame(true);
				ActiveLeftHandWeapon = nullptr;
			}
		}
		else if (Hand == 1)
		{
			if (ActiveRightHandWeapon && ActiveRightHandWeapon->ItemName == ARItemID)
			{
				//we don't want to move item back to inventory.
				//we just need to reset pointer to weapon, and un attach mesh.
				ActiveRightHandWeaponStruct.ItemKey = NAME_None;
				ActiveRightHandWeaponStruct.ItemIndex = INDEX_NONE;
				ActiveRightHandWeapon->SetActorHiddenInGame(true);
				ActiveRightHandWeapon = nullptr;
			}
		}
	}
}
void UAREquipmentComponent::ServerUnEquipWeapon_Implementation(FName ARItemID, int32 Hand)
{
	UnEquipWeapon(ARItemID, Hand);
}
bool UAREquipmentComponent::ServerUnEquipWeapon_Validate(FName ARItemID, int32 Hand)
{
	return true;
}
