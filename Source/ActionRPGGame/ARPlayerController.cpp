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

#include "JsonDataAsset/JsonDataAsset.h"
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

	if (PlayerCameraManager)
	{
		PlayerCameraManager->ViewPitchMax = 70.0f;
	}

	//initialize slots for left hand weapon.
	//for (int32 i = 0; i < 4; i++)
	//{
	//	FInventorySlot in;
	//	in.ItemID = NAME_None;
	//	in.SlotID = i;
	//	in.EEquipmentSlot = EEquipmentSlot::Item_LeftHandOne;
	//	in.ItemSlot = EItemSlot::Item_Weapon;
	//	LeftHandWeapons.Add(in);
	//}
	//LeftHandWeaponsUpdated = false;

	//for (int32 i = 0; i < 4; i++)
	//{
	//	FInventorySlot in;
	//	in.ItemID = NAME_None;
	//	in.SlotID = i;
	//	in.EEquipmentSlot = EEquipmentSlot::Item_RightHandOne;
	//	in.ItemSlot = EItemSlot::Item_Weapon;
	//	RightHandWeapons.Add(in);
	//}
	//RightHandWeaponsUpdated = false;

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
	FItemEntry item1 = TestItems->GetItemFromArray(0);
	wep1.ItemID = item1.Key;
	wep1.ItemSlot = EItemSlot::Item_Weapon;
	wep1.ItemIndex = 0;
	Inventory->AddItemToInventory(wep1);

	FItemEntry item2 = TestItems->GetItemFromArray(1);
	FInventorySlot wep2;
	wep2.ItemIndex = 1;
	wep2.ItemID = item2.Key;
	wep2.ItemSlot = EItemSlot::Item_Weapon;
	Inventory->AddItemToInventory(wep2);

	//FItemEntry item1 = TestItems->GetItemFromArray(0);

	//FARItemInfo* itemptr = GlobalConfig::item->GetItemPtr("key");
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
		SetActiveAbility(ActionBarOne[0].Ability.Get());
		//ActiveAbility = ActionBarOne[0].Ability.Get();
		//ActiveAbility->Initialize();
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
		SetActiveAbility(ActionBarOne[1].Ability.Get());
		//ActiveAbility = ActionBarOne[1].Ability.Get();
		//ActiveAbility->Initialize();
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

void AARPlayerController::ServerSetActiveAbility_Implementation(class AARAbility* AbilityIn)
{
	SetActiveAbility(AbilityIn);
}
bool AARPlayerController::ServerSetActiveAbility_Validate(class AARAbility* AbilityIn)
{
	return true;
}
void AARPlayerController::SetActiveAbility(class AARAbility* AbilityIn)
{
	if (Role < ROLE_Authority)
	{
		ServerSetActiveAbility(AbilityIn);
	}
	else
	{
		ActiveAbility = AbilityIn;
		ActiveAbility->Initialize();
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
					abilityBar.Ability->Initialize();

					OnAbilityEquiped.Broadcast(abilityBar.Ability.Get());
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


void AARPlayerController::OnRep_ActionBarOne()
{
	UpdateActionBarOne = true;
}
void AARPlayerController::OnRep_AbilityInventory()
{
	UpdateAbilityInventory = true;
}

void AARPlayerController::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AARPlayerController, AbilityInventory, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AARPlayerController, ActionBarOne, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(AARPlayerController, UIDamage, COND_OwnerOnly);

	DOREPLIFETIME(AARPlayerController, ActiveAbility);
}

void AARPlayerController::OnRep_UIDamage()
{
	//AARHUD* hud = Cast<AARHUD>(GetHUD());
	//if (hud)
	//{
	//	hud->PawnDamaged(UIDamage.Location, UIDamage.Value, UDamageType::StaticClass());
	//}
}