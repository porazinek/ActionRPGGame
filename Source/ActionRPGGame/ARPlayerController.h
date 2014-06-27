// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "Types/ARStructTypes.h"

#include "Types/ARInvItem.h"

#include "Slate.h"

#include "ARPlayerController.generated.h"

UCLASS()
class AARPlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()
private:
	UPROPERTY()
	class AARCharacter* ARCharacter;
public:

	virtual void SetPawn(APawn* InPawn) override;

	/* Input **/
	virtual void SetupInputComponent() override;
	void InputActionButtonOne();
	void InputActionButtonTwo();

	/* 
		Route weapon fire trough Controller
		Will be useful, when later there will be multiple pawns.
		Like vechicles, so we can just use the same binding to fire vechicle actions.
	*/
	void InputFireLeftWeapon();
	void InputFireRightWeapon();
	/* GUI Input **/
	void SetInventoryVisibility();
	EVisibility InventoryVisibility;

	void SetCharacterSheetVisibility();
	EVisibility CharacterSheetVisibility;

	void SetAbilityInventoryVisibility();
	EVisibility AbilityInventoryVisibility;

	/* Character Sheet **/
	//Left Hand Weapon
	UPROPERTY(ReplicatedUsing=OnRep_LeftHandWeapons) //not sure but I think we shouldn't really replicate this back.
		TArray<FInventorySlot> LeftHandWeapons;
	UFUNCTION()
		void OnRep_LeftHandWeapons();
	bool LeftHandWeaponsUpdated;
	void AddLeftHandWeapon(FInventorySlot Weapon, int32 SlotID);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAddLeftHandWeapon(FInventorySlot Weapon, int32 SlotID);

	//Right Hand Weapon
	UPROPERTY(ReplicatedUsing = OnRep_RightHandWeapons)
		TArray<FInventorySlot> RightHandWeapons;
	UFUNCTION()
		void OnRep_RightHandWeapons();
	bool RightHandWeaponsUpdated;
	void AddRightHandWeapon(FInventorySlot Weapon, int32 SlotID);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAddRightHandWeapon(FInventorySlot Weapon, int32 SlotID);

	//Equiped Items
	UPROPERTY()
		TArray<FInventorySlot> EquippedItems;

	/* Ability Inventory - Spellbook, mele action bla bla, got the point **/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_AbilityInventory, Category = "Abilities")
		TArray <FAbilityInfo> AbilityInventory;
	UFUNCTION()
		void OnRep_AbilityInventory();
		bool UpdateAbilityInventory;
	UPROPERTY(ReplicatedUsing = OnRep_ActionBarOne)
		TArray<FAbilityInfo> ActionBarOne;
	UFUNCTION()
		void OnRep_ActionBarOne();
	bool UpdateActionBarOne;

	bool AddAbilityToInventory(FAbilityInfo AbilityIn);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAddAbilityToInventory(FAbilityInfo AbilityIn);

	UFUNCTION(BlueprintCallable, Category = "Ability")
		void AddAbilityToActionBar(FAbilityInfo AbilityIn, int32 SlotID);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAddAbilityToActionBar(FAbilityInfo AbilityIn, int32 SlotID);

	/* Inventory **/
	UPROPERTY(ReplicatedUsing = OnRep_InventoryChanged)
		TArray<FInventorySlot> InventorySmall;
	
	UPROPERTY(Replicated)
		int32 MaxInventorySize;

	UFUNCTION()
		void OnRep_InventoryChanged();

	void AddItemToInventory(FInventorySlot Item);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAddItemToInventory(FInventorySlot Item);

	void AddItemToInventoryOnSlot(FInventorySlot Item, int32 SlotID);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAddItemToInventoryOnSlot(FInventorySlot Item, int32 SlotID);

	bool RemoveItemFromInventory(FName ItemID, int32 SlotID);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRemoveItemFromInventory(FName ItemID, int32 SlotID);
	
	UFUNCTION(Client, Reliable)
		void ClientSetInventoryChanged();

	void SetInventoryChanged();
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetInventoryChanged();
	UPROPERTY(Replicated)
	bool IsInventoryChanged;
};



