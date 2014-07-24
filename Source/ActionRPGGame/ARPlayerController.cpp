// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "ActionRPGGame.h"

#include "Componenets/AREquipmentComponent.h"
#include "ARCharacter.h"
#include "Items/ARWeapon.h"
#include "Abilities/ARAbility.h"
#include "../Componenets/ARAttributeBaseComponent.h"

#include "Items/ARInventoryComponent.h"

#include "HUD/ARHUD.h"

#include "Effects/AREffectType.h"

#include "Net/UnrealNetwork.h"

#include "ARPlayerController.h"

AARPlayerController::AARPlayerController(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

	InventoryVisibility = EVisibility::Collapsed;
	CharacterSheetVisibility = EVisibility::Collapsed;
	AbilityInventoryVisibility = EVisibility::Collapsed;

	Attributes = PCIP.CreateDefaultSubobject<UARAttributeBaseComponent>(this, TEXT("Attributes"));
	Attributes->SetIsReplicated(true);
	Attributes->SetNetAddressable();

	Inventory = PCIP.CreateDefaultSubobject<UARInventoryComponent>(this, TEXT("Inventory"));
	Inventory->SetIsReplicated(true);
	Inventory->SetNetAddressable();

	//bReplicates = true;
	//bOnlyRelevantToOwner = false;
	UpdateActionBarOne = false;
	UpdateAbilityInventory = false;
	for (int8 i = 0; i < 5; i++)
	{
		FAbilityInfo ab;
		ab.SlotID = i;
		ab.Ability = nullptr;
		ActionBarOne.Add(ab);
	}

	IsInventoryChanged = false;
	if (PlayerCameraManager)
	{
		PlayerCameraManager->ViewPitchMax = 70.0f;
	}
	MaxInventorySize = 16;
	InventorySmall.Reserve(MaxInventorySize);
	for (int32 i = 0; i < MaxInventorySize; i++)
	{
		FInventorySlot slot;
		slot.SlotID = i;
		slot.ItemID = NAME_None; //no item on that slot.
		slot.ItemSlot = EItemSlot::Item_Inventory;
		slot.EEquipmentSlot = EEquipmentSlot::Item_Inventory;
		InventorySmall.Add(slot);
	};
	//initialize slots for left hand weapon.
	for (int32 i = 0; i < 4; i++)
	{
		FInventorySlot in;
		in.ItemID = NAME_None;
		in.SlotID = i;
		in.EEquipmentSlot = EEquipmentSlot::Item_LeftHandOne;
		in.ItemSlot = EItemSlot::Item_Weapon;
		LeftHandWeapons.Add(in);
	}
	LeftHandWeaponsUpdated = false;

	for (int32 i = 0; i < 4; i++)
	{
		FInventorySlot in;
		in.ItemID = NAME_None;
		in.SlotID = i;
		in.EEquipmentSlot = EEquipmentSlot::Item_RightHandOne;
		in.ItemSlot = EItemSlot::Item_Weapon;
		RightHandWeapons.Add(in);
	}
	RightHandWeaponsUpdated = false;

	PlayerCameraManagerClass = AARPlayerCameraManager::StaticClass();

	//Inventory.AddZeroed(MaxInventorySize);
}
void AARPlayerController::BeginPlay()
{
	Super::BeginPlay();
	/*
		For whatever reason, telling it to replicate in constructor is not enough.
		We must do so also in BeginPlay().

		Maybe somwhere else in hierarchy call would also work, like PostInitializeComponents.
	*/
	Inventory->SetIsReplicated(true);
}
void AARPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	ARCharacter = Cast<AARCharacter>(InPawn);
}

void AARPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//GUI input
	InputComponent->BindAction("ShowInventory", IE_Pressed, this, &AARPlayerController::SetInventoryVisibility);
	InputComponent->BindAction("ShowCharacterSheet", IE_Pressed, this, &AARPlayerController::SetCharacterSheetVisibility);
	InputComponent->BindAction("ShowAbilityInventory", IE_Pressed, this, &AARPlayerController::SetAbilityInventoryVisibility);

	InputComponent->BindAction("ActionButtonOne", IE_Pressed, this, &AARPlayerController::InputActionButtonOne);
	InputComponent->BindAction("ActionButtonTwo", IE_Pressed, this, &AARPlayerController::InputActionButtonTwo);

	InputComponent->BindAction("ActivateAbility", IE_Pressed, this, &AARPlayerController::InputActivateAbility);

	InputComponent->BindAction("FireLeftWeapon", IE_Pressed, this, &AARPlayerController::InputFireLeftWeapon);
	InputComponent->BindAction("FireRightWeapon", IE_Pressed, this, &AARPlayerController::InputFireRightWeapon);
	InputComponent->BindAction("FireLeftWeapon", IE_Released, this, &AARPlayerController::InputStopFireLeftWeapon);
	InputComponent->BindAction("FireRightWeapon", IE_Released, this, &AARPlayerController::InputStopFireRightWeapon);

	InputComponent->BindAction("SwapLeftWeapon", IE_Pressed, this, &AARPlayerController::InputSwapLeftWeapon);
	InputComponent->BindAction("SwapRightWeapon", IE_Pressed, this, &AARPlayerController::InputSwapRightWeapon);
	InputComponent->BindAction("AddWeapons", IE_Pressed, this, &AARPlayerController::InputTempAddWeapons);
}

void AARPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AARPlayerController::InputTempAddWeapons()
{
	FInventorySlot wep1;
	wep1.ItemID = "RedWeapon";
	wep1.ItemSlot = EItemSlot::Item_Weapon;
	Inventory->AddItemToInventory(wep1);
	FInventorySlot wep2;
	wep2.ItemID = "TestWeapon";
	wep2.ItemSlot = EItemSlot::Item_Weapon;
	Inventory->AddItemToInventory(wep2);
}

void AARPlayerController::InputActivateAbility()
{
	if (ActiveAbility)
	{
		IIARActionState* actionInterface = InterfaceCast<IIARActionState>(ActiveAbility);
		if (actionInterface)
		{
			actionInterface->InputPressed();
		}
	}
}

void AARPlayerController::InputActionButtonOne()
{
	//second press should deactivate ability and remove it from here.
	if (ActionBarOne[0].Ability.IsValid())
	{
		ActiveAbility = ActionBarOne[0].Ability.Get();
		ActiveAbility->Initialize();
		/*IIARActionState* actionInterface = InterfaceCast<IIARActionState>(ActionBarOne[0].Ability.Get());
		if (actionInterface)
		{
		actionInterface->InputPressed();
		}*/
	}
}
void AARPlayerController::InputActionButtonTwo()
{
	if (ActionBarOne[1].Ability.IsValid())
	{
		ActiveAbility = ActionBarOne[1].Ability.Get();
		ActiveAbility->Initialize();
		//IIARActionState* actionInterface = InterfaceCast<IIARActionState>(ActionBarOne[1].Ability.Get());
		//if (actionInterface)
		//{
		//	actionInterface->InputPressed();
		//}
	}
}
void AARPlayerController::InputFireLeftWeapon()
{
	if (ARCharacter)
	{
		ARCharacter->InputFireLeftWeapon();
	}
}
void AARPlayerController::InputFireRightWeapon()
{
	if (ARCharacter)
	{
		ARCharacter->InputFireRightWeapon();
	}
}
void AARPlayerController::InputStopFireLeftWeapon()
{
	if (ARCharacter)
	{
		ARCharacter->InputStopFireLeftWeapon();
	}
}
void AARPlayerController::InputStopFireRightWeapon()
{
	if (ARCharacter)
	{
		ARCharacter->InputStopFireRightWeapon();
	}
}

void AARPlayerController::InputSwapRightWeapon()
{
	if (ARCharacter)
	{
		ARCharacter->InputSwapRightWeapon();
	}
}
void AARPlayerController::InputSwapLeftWeapon()
{
	if (ARCharacter)
	{
		ARCharacter->InputSwapLeftWeapon();
	}
}

void AARPlayerController::InputPickupItem()
{

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

void AARPlayerController::SetAbilityInventoryVisibility()
{
	if (AbilityInventoryVisibility == EVisibility::Collapsed)
	{
		AbilityInventoryVisibility = EVisibility::Visible;
	}
	else
	{
		AbilityInventoryVisibility = EVisibility::Collapsed;
	}
}

void AARPlayerController::AddAbilityToActionBar(FAbilityInfo AbilityIn, int32 SlotID)
{
	if (Role < ROLE_Authority)
	{
		ServerAddAbilityToActionBar(AbilityIn, SlotID);
	}
	else
	{
		if (ActionBarOne.Num() <= 5)
		{
			//for (FAbilityInfo& ability : OwningController->AbilityInventory)
			//{
			//if (ability.AbilityName == AbilityIn.AbilityName)
			//{
			for (FAbilityInfo& abilityBar : ActionBarOne)
			{
				if (abilityBar.SlotID == SlotID)
				{
					if (abilityBar.Ability.IsValid())
					{
						abilityBar.Ability->Destroy();
						abilityBar.Ability.Reset();
					}

					if (!AbilityIn.AbilityType)
						return;

					FActorSpawnParameters SpawnInfo;
					SpawnInfo.bNoCollisionFail = true;

					AARAbility* tempAbi = GetWorld()->SpawnActor<AARAbility>(AbilityIn.AbilityType, SpawnInfo);
					tempAbi->SetOwner(GetPawn());
					tempAbi->OwningCharacter = ARCharacter;
					abilityBar.AbilityType = AbilityIn.AbilityType;
					abilityBar.AbilityName = AbilityIn.AbilityName;
					abilityBar.Ability = tempAbi;

					UpdateActionBarOne = true;
				}
			}
			//}
			//}
		}
	}
}
void AARPlayerController::ServerAddAbilityToActionBar_Implementation(FAbilityInfo AbilityIn, int32 SlotID)
{
	AddAbilityToActionBar(AbilityIn, SlotID);
}

bool AARPlayerController::ServerAddAbilityToActionBar_Validate(FAbilityInfo AbilityIn, int32 SlotID)
{
	return true;
}

bool AARPlayerController::AddAbilityToInventory(FAbilityInfo AbilityIn)
{
	if (Role < ROLE_Authority)
	{
		ServerAddAbilityToInventory(AbilityIn);
	}
	else
	{
		//for testing purposes we will just add abilities directly.
		//in reality it should query Data Table and pull ability from here
		//so we wouldn't even need to send entire struct just ID of ability
		//and construct correct struct on server.
		AbilityIn.SlotID = AbilityInventory.Num() + 1;
		AbilityInventory.Add(AbilityIn);
		UpdateAbilityInventory = true;
		return true;
	}
	return false;
}
void AARPlayerController::ServerAddAbilityToInventory_Implementation(FAbilityInfo AbilityIn)
{
	AddAbilityToInventory(AbilityIn);
}
bool AARPlayerController::ServerAddAbilityToInventory_Validate(FAbilityInfo AbilityIn)
{
	return true;
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
		if (Feats.Num() > 0)
		{
			float g = 0;
		}
		if (InventorySmall.Num() <= MaxInventorySize)
		{
			for (FInventorySlot& item : InventorySmall)
			{
				if (item.ItemID.IsNone())
				{
					item.ItemID = Item.ItemID;
					item.ItemSlot = Item.ItemSlot;
					item.EEquipmentSlot = Item.EEquipmentSlot;
					IsInventoryChanged = true;
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
				if (item.SlotID == SlotID && item.ItemID != NAME_None)
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
							IsInventoryChanged = true;
							ClientSetInventoryChanged();
							//OnRep_InventoryChanged();
							return;
						}
					}
					IsInventoryChanged = true;
					//OnRep_InventoryChanged();
					return;
				}
				if (item.ItemID == NAME_None && item.SlotID == SlotID)
				{
					item.ItemID = Item.ItemID;
					item.ItemSlot = Item.ItemSlot;
					item.EEquipmentSlot = Item.EEquipmentSlot;
					RemoveItemFromInventory(Item.ItemID, Item.OldSlotID);
					IsInventoryChanged = true;
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
void AARPlayerController::ServerRemoveItemFromInventory_Implementation(FName ItemID, int32 SlotID)
{
	RemoveItemFromInventory(ItemID, SlotID);
}
bool AARPlayerController::ServerRemoveItemFromInventory_Validate(FName ItemID, int32 SlotID)
{
	return true;
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

void AARPlayerController::AddLeftHandWeapon(FInventorySlot Weapon, int32 SlotID)
{
	if (Role < ROLE_Authority)
	{
		ServerAddLeftHandWeapon(Weapon, SlotID);
	}
	else
	{
		for (FInventorySlot& weapon : LeftHandWeapons)
		{
			if (weapon.SlotID == SlotID && weapon.ItemID != NAME_None)
			{
				FInventorySlot oldItemTemp = weapon;
				weapon.ItemID = Weapon.ItemID;
				weapon.ItemSlot = Weapon.ItemSlot;
				weapon.EEquipmentSlot = Weapon.EEquipmentSlot;
				for (FInventorySlot& oldItem : InventorySmall)
				{
					if (weapon.SlotID == oldItem.SlotID)
					{
						oldItem.ItemID = oldItemTemp.ItemID;
						oldItem.ItemSlot = oldItemTemp.ItemSlot;
						oldItem.EEquipmentSlot = oldItemTemp.EEquipmentSlot;
						return;
					}
				}
				LeftHandWeaponsUpdated = true;
				//OnRep_InventoryChanged();
				return;
			}
			if (weapon.ItemID.IsNone() && weapon.SlotID == SlotID)
			{
				weapon.ItemID = Weapon.ItemID;
				weapon.ItemSlot = Weapon.ItemSlot;
				weapon.EEquipmentSlot = Weapon.EEquipmentSlot;
				LeftHandWeaponsUpdated = true;
				return;
			}
		}
	}
}
void AARPlayerController::ServerAddLeftHandWeapon_Implementation(FInventorySlot Weapon, int32 SlotID)
{
	AddLeftHandWeapon(Weapon, SlotID);
}
bool AARPlayerController::ServerAddLeftHandWeapon_Validate(FInventorySlot Weapon, int32 SlotID)
{
	return true;
}

bool AARPlayerController::RemoveLeftHandWeapon(FName ItemID, int32 SlotID)
{
	if (Role < ROLE_Authority)
	{
		ServerRemoveLeftHandWeapon(ItemID, SlotID);
	}
	else
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
				return true;
			}
		}
	}
	return false;
}
void AARPlayerController::ServerRemoveLeftHandWeapon_Implementation(FName ItemID, int32 SlotID)
{
	RemoveLeftHandWeapon(ItemID, SlotID);
}
bool AARPlayerController::ServerRemoveLeftHandWeapon_Validate(FName ItemID, int32 SlotID)
{
	return true;
}

void AARPlayerController::AddRightHandWeapon(FInventorySlot Weapon, int32 SlotID)
{
	if (Role < ROLE_Authority)
	{
		ServerAddRightHandWeapon(Weapon, SlotID);
	}
	else
	{
		for (FInventorySlot& weapon : RightHandWeapons)
		{
			if (weapon.SlotID == SlotID && weapon.ItemID != NAME_None)
			{
				FInventorySlot oldItemTemp = weapon;
				weapon.ItemID = Weapon.ItemID;
				weapon.ItemSlot = Weapon.ItemSlot;
				weapon.EEquipmentSlot = Weapon.EEquipmentSlot;
				for (FInventorySlot& oldItem : InventorySmall)
				{
					if (weapon.SlotID == oldItem.SlotID)
					{
						oldItem.ItemID = oldItemTemp.ItemID;
						oldItem.ItemSlot = oldItemTemp.ItemSlot;
						oldItem.EEquipmentSlot = oldItemTemp.EEquipmentSlot;
						return;
					}
				}
				RightHandWeaponsUpdated = true;
				//OnRep_InventoryChanged();
				return;
			}
			if (weapon.ItemID.IsNone() && weapon.SlotID == SlotID)
			{
				weapon.ItemID = Weapon.ItemID;
				weapon.ItemSlot = Weapon.ItemSlot;
				weapon.EEquipmentSlot = Weapon.EEquipmentSlot;

				RightHandWeaponsUpdated = true;
				return;
			}
		}
	}
}
void AARPlayerController::ServerAddRightHandWeapon_Implementation(FInventorySlot Weapon, int32 SlotID)
{
	AddRightHandWeapon(Weapon, SlotID);
}
bool AARPlayerController::ServerAddRightHandWeapon_Validate(FInventorySlot Weapon, int32 SlotID)
{
	return true;
}

bool AARPlayerController::RemoveRightHandWeapon(FName ItemID, int32 SlotID)
{
	if (Role < ROLE_Authority)
	{
		ServerRemoveRightHandWeapon(ItemID, SlotID);
	}
	else
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
				return true;
			}
		}
	}
	return false;
}
void AARPlayerController::ServerRemoveRightHandWeapon_Implementation(FName ItemID, int32 SlotID)
{
	RemoveRightHandWeapon(ItemID, SlotID);
}
bool AARPlayerController::ServerRemoveRightHandWeapon_Validate(FName ItemID, int32 SlotID)
{
	return true;
}

void AARPlayerController::OnRep_ActionBarOne()
{
	UpdateActionBarOne = true;
}
void AARPlayerController::OnRep_AbilityInventory()
{
	UpdateAbilityInventory = true;
}
void AARPlayerController::OnRep_LeftHandWeapons()
{
	LeftHandWeaponsUpdated = true;
}
void AARPlayerController::OnRep_RightHandWeapons()
{
	RightHandWeaponsUpdated = true;
}
void AARPlayerController::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AARPlayerController, MaxInventorySize, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(AARPlayerController, InventorySmall, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AARPlayerController, IsInventoryChanged, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(AARPlayerController, AbilityInventory, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AARPlayerController, ActionBarOne, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(AARPlayerController, LeftHandWeapons, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AARPlayerController, RightHandWeapons, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AARPlayerController, UIDamage, COND_OwnerOnly);
}

void AARPlayerController::OnRep_UIDamage()
{
	//AARHUD* hud = Cast<AARHUD>(GetHUD());
	//if (hud)
	//{
	//	hud->PawnDamaged(UIDamage.Location, UIDamage.Value, UDamageType::StaticClass());
	//}
}