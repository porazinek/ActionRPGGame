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
public:
	//virtual void PostInitializeComponents() OVERRIDE;

	/* Input **/
	virtual void SetupInputComponent() override;

	/* GUI Input **/
	void SetInventoryVisibility();
	EVisibility InventoryVisibility;

	void SetCharacterSheetVisibility();
	EVisibility CharacterSheetVisibility;

	/* Ability Inventory - Spellbook, mele action bla bla, got the point **/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		TArray <FAbilityInfo> AbilityInventory;

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
	UPROPERTY(ReplicatedUsing=OnRep_InventoryChanged)
		TArray<FARItemInfo> Inventory;

	UPROPERTY(ReplicatedUsing = OnRep_InventoryChanged)
		TArray<UObject*> InventoryObj;

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
	
	//prolly no longer needed.
	void SwapItemPosition(FARItemInfo& Item, int32 NewIndex);

	UFUNCTION(Client, Reliable)
		void ClientSetInventoryChanged();

	void SetInventoryChanged();
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetInventoryChanged();
	UPROPERTY(Replicated)
	bool IsInventoryChanged;
};



