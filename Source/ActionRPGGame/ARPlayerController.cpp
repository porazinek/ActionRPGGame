// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "ActionRPGGame.h"

#include "Componenets/AREquipmentComponent.h"
#include "ARCharacter.h"
#include "Items/ARWeapon.h"
#include "Abilities/ARAbility.h"
#include "Abilities/ARAbilityComponent.h"
#include "../Componenets/ARAttributeBaseComponent.h"


#include "Items/ARInventoryComponent.h"

#include "HUD/ARHUD.h"

#include "Effects/AREffectType.h"

#include "Net/UnrealNetwork.h"

#include "BlueprintLibrary/ARTraceStatics.h"
#include "Items/ARItemPickup.h"

#include "ARPlayerController.h"

AARPlayerController::AARPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	InventoryVisibility = EVisibility::Collapsed;
	CharacterSheetVisibility = EVisibility::Collapsed;
	AbilityInventoryVisibility = EVisibility::Collapsed;

	Abilities = ObjectInitializer.CreateDefaultSubobject<UARAbilityComponent>(this, TEXT("Abilities"));
	Abilities->SetIsReplicated(true);
	Abilities->SetNetAddressable();

	Attributes = ObjectInitializer.CreateDefaultSubobject<UARAttributeBaseComponent>(this, TEXT("Attributes"));
	Attributes->SetIsReplicated(true);
	Attributes->SetNetAddressable();

	Inventory = ObjectInitializer.CreateDefaultSubobject<UARInventoryComponent>(this, TEXT("Inventory"));
	Inventory->SetIsReplicated(true);
	Inventory->SetNetAddressable();
	AimMode = true;
	FreeTargetMode = false;
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


	PlayerCameraManagerClass = AARPlayerCameraManager::StaticClass();

	//Inventory.AddZeroed(MaxInventorySize);
}
void AARPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	//if (LocalPlayer)
	//{
	//	FViewport* Viewport = LocalPlayer->ViewportClient->Viewport;
	//	
	//	FIntPoint ViewportSize = Viewport->GetSizeXY();
	//	//FVector2D ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();
	//	FVector2D MousePosition;
	//	//just testing probabaly need less hardcoded math, for the subtraction part.
	//	MousePosition.X = (ViewportSize.X * 0.5);
	//	MousePosition.Y = (ViewportSize.Y * 0.5);
	//	Viewport->SetMouse(600, 360);
	//	GEngine->AddOnScreenDebugMessage(1, 10, FColor::Red, FString::FormatAsNumber(MousePosition.X));
	//	//GEngine->AddOnScreenDebugMessage(0, 2, FColor::Blue, FString::FormatAsNumber(MousePosition.Y));
	//}
	/*
		For whatever reason, telling it to replicate in constructor is not enough.
		We must do so also in BeginPlay().

		Maybe somwhere else in hierarchy call would also work, like PostInitializeComponents.
	*/
	Inventory->SetIsReplicated(true);
	Abilities->SetIsReplicated(true);
	
}
void AARPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	ARCharacter = Cast<AARCharacter>(InPawn);
	Abilities->OwningCharacter = ARCharacter;
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
	
	InputComponent->BindAction("ReloadLeftWeapon", IE_Released, this, &AARPlayerController::InputReloadLeftWeapon);
	InputComponent->BindAction("ReloadRightWeapon", IE_Released, this, &AARPlayerController::InputReloadRightWeapon);

	InputComponent->BindAction("SwapLeftWeapon", IE_Pressed, this, &AARPlayerController::InputSwapLeftWeapon);
	InputComponent->BindAction("SwapRightWeapon", IE_Pressed, this, &AARPlayerController::InputSwapRightWeapon);

	InputComponent->BindAction("PickupItem", IE_Pressed, this, &AARPlayerController::InputPickupItem);

	InputComponent->BindAction("AddWeapons", IE_Pressed, this, &AARPlayerController::InputTempAddWeapons);

	InputComponent->BindAction("ToggleMouseMode", IE_Pressed, this, &AARPlayerController::InputToggleMouseMode);
	InputComponent->BindAction("ToggleMouseTargeting", IE_Pressed, this, &AARPlayerController::InputToggleMouseTargeting);
}

void AARPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AARPlayerController::InputToggleMouseMode()
{
	if (AimMode)
	{
		bEnableClickEvents = true;
		bEnableMouseOverEvents = true;
		bShowMouseCursor = true;
		SetIgnoreLookInput(true);
		SetIgnoreMoveInput(true);
		AimMode = false;
	}
	else
	{
		bEnableClickEvents = false;
		bEnableMouseOverEvents = false;
		bShowMouseCursor = false;
		SetIgnoreLookInput(false);
		SetIgnoreMoveInput(false);
		AimMode = true;
		ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
		FViewport* Viewport = LocalPlayer->ViewportClient->Viewport;
		FIntPoint ViewSize = Viewport->GetSizeXY();
		//FVector2D ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();
		FVector2D MousePosition;
		//just testing probabaly need less hardcoded math, for the subtraction part.
		MousePosition.X = ViewSize.X * 0.5f - 40.0f;
		MousePosition.Y = ViewSize.Y * 0.5f + 56.0f;

		Viewport->SetMouse(MousePosition.X, MousePosition.Y);
	}
}
void AARPlayerController::InputToggleMouseTargeting()
{
	if (FreeTargetMode == true)
	{

		AARHUD* MyHUD = Cast<AARHUD>(GetHUD());
		SetIgnoreLookInput(true);
		
		ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
		FViewport* Viewport = LocalPlayer->ViewportClient->Viewport;
		FIntPoint ViewSize = Viewport->GetSizeXY();
		//FVector2D ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();
		FVector2D MousePosition;
		//just testing probabaly need less hardcoded math, for the subtraction part.
		MousePosition.X = ViewSize.X * 0.5f - 50.0f;
		MousePosition.Y = ViewSize.Y * 0.5f + 56.0f;

		Viewport->SetMouse(MousePosition.X, MousePosition.Y);

		
		if (MyHUD)
		{
			MyHUD->TargetModeSwaped = true;
		}
		FreeTargetMode = false;
		return;
	}
	if (FreeTargetMode == false)
	{
		SetIgnoreLookInput(true);
		AARHUD* MyHUD = Cast<AARHUD>(GetHUD());
		
		ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
		FViewport* Viewport = LocalPlayer->ViewportClient->Viewport;
		FIntPoint ViewSize = Viewport->GetSizeXY();
		
		//FVector2D ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();
		FVector2D MousePosition;
		//just testing probabaly need less hardcoded math, for the subtraction part.
		MousePosition.X = ViewSize.X * 0.5f - 50.0f;
		MousePosition.Y = ViewSize.Y * 0.5f + 56.0f;

		Viewport->SetMouse(MousePosition.X, MousePosition.Y);
		if (MyHUD)
		{
			MyHUD->TargetModeSwaped = true;
		}
		FreeTargetMode = true;
		return;
	}
}
void AARPlayerController::InputTempAddWeapons()
{
	FARDragDropInfo wep1;
	FItemEntry item1 = TestItems->GetItemFromArray(0);
	wep1.ItemKey = item1.Key;
	wep1.ItemSlot = EItemSlot::Item_Weapon;
	wep1.ItemIndex = 0;
	Inventory->AddItemToInventory(wep1);

	FItemEntry item2 = TestItems->GetItemFromArray(1);
	FARDragDropInfo wep2;
	wep2.ItemIndex = 1;
	wep2.ItemKey = item2.Key;
	wep2.ItemSlot = EItemSlot::Item_Weapon;
	Inventory->AddItemToInventory(wep2);

	//FItemEntry item1 = TestItems->GetItemFromArray(0);

	//FARItemInfo* itemptr = GlobalConfig::item->GetItemPtr("key");
}

void AARPlayerController::InputActivateAbility()
{
	if (Abilities->ActiveAbility)
	{
		IIARActionState* actionInterface = Cast<IIARActionState>(Abilities->ActiveAbility);
		if (actionInterface)
		{
			actionInterface->InputPressed();
		}
	}
}

void AARPlayerController::InputActionButtonOne()
{
	//second press should deactivate ability and remove it from here.
//	if (ActionBarOne[0].Ability.IsValid())
	//{
		Abilities->SetActiveAction(Abilities->ActionBars.ActionBars[0].ActionSlots[0]);
	//	SetActiveAbility(ActionBarOne[0].Ability.Get());
//	}
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

void AARPlayerController::InputReloadLeftWeapon()
{
	if (ARCharacter)
	{
		ARCharacter->InputReloadLeftWeapon();
	}
}
void AARPlayerController::InputReloadRightWeapon()
{
	if (ARCharacter)
	{
		ARCharacter->InputReloadRightWeapon();
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
	PickUpItem();
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


void AARPlayerController::PickUpItem()
{
	if (Role < ROLE_Authority)
	{
		ServerPickUpItem();
	}
	else
	{
		FHitResult hit = UARTraceStatics::GetHitResult(1000, NAME_None, GetPawn(), false, false, EARTraceType::Trace_Weapon, EWeaponHand::NoWeapon);

		AARItemPickup* pickup = Cast<AARItemPickup>(hit.GetActor());
		if (pickup)
		{
			pickup->GiveAllItemsTo(this);
			ClientPickUpItem(pickup);
			//OnItemPicked.ExecuteIfBound(pickup);
		}
	}
}
void AARPlayerController::ClientPickUpItem_Implementation(class AARItemPickup* PickupItem)
{
	OnItemPicked.ExecuteIfBound(PickupItem);
}

void AARPlayerController::ServerPickUpItem_Implementation()
{
	PickUpItem();
}
bool AARPlayerController::ServerPickUpItem_Validate()
{
	return true;
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