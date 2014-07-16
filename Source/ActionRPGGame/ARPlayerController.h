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
	/*
	Hook for controller. Controller doesn't have any attributes per se,
	but attribute component also implements notifications and events,
	and we'd like to access them from owning conroller as well.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
		TSubobjectPtr<class UARAttributeBaseComponent> Attributes;
	virtual void SetPawn(APawn* InPawn) override;

	virtual void PostInitializeComponents() override;

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
	void InputStopFireLeftWeapon();
	void InputStopFireRightWeapon();

	void InputTempAddWeapons();

	void InputSwapLeftWeapon();
	void InputSwapRightWeapon();


	//debug

	void InputAddFeat();
	void AddFeat();
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAddFeat();

	/* GUI Input **/
	void SetInventoryVisibility();
	EVisibility InventoryVisibility;

	void SetCharacterSheetVisibility();
	EVisibility CharacterSheetVisibility;

	void SetAbilityInventoryVisibility();
	EVisibility AbilityInventoryVisibility;

	/*
		Temporary test for Effects which are going to be feats.
		Base functionality test.
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Feats")
		TArray<class UAREffectType*> Feats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feats")
		TArray<TSubclassOf<class UAREffectType>> FeatClasses;
	/*
		Everything regarding currently equiped items and inventory is handled on server.
		Every time user is trying to change something in inventory there is RPC call made from client 
		to server, where client request operation on Item.

		It could potentialy get quite heavy, because all RPC calls are reliable,
		My assumption here is, that operations on inventory in most extreme cases, are not
		that often, and we don't want risk user operation on his own items will be lost
		somewhere over network.
	*/
	/* Character Sheet **/
	//Left Hand Weapon
	/*
		List of weapon which can be equiped to left hand
	*/
	UPROPERTY(ReplicatedUsing=OnRep_LeftHandWeapons) //not sure but I think we shouldn't really replicate this back.
		TArray<FInventorySlot> LeftHandWeapons;
	UFUNCTION()
		void OnRep_LeftHandWeapons();
	/*
		Indicates whether there is some change in LeftHandWeapons
	*/
	bool LeftHandWeaponsUpdated;
	/* 
		[client] [server] - Add left hand weapon to LeftHandWeapons
	*/
	void AddLeftHandWeapon(FInventorySlot Weapon, int32 SlotID);

	/* 
		[server] - Calls AddLeftHandWeapon On server. 
	*/
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAddLeftHandWeapon(FInventorySlot Weapon, int32 SlotID);

	/* 
		[client] [server] - Remove left hand weapon to list of equiped weapons.
		Return true if something has been removed.
	*/
	bool RemoveLeftHandWeapon(FName ItemID, int32 SlotID);

	/* 
		[server] - Calls RemoveLeftHandWeapon On server. 
		
		@param ItemID - ID of item remove.
		@param SlotID - ID of Inventory slot, which item occupy.
	*/
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRemoveLeftHandWeapon(FName ItemID, int32 SlotID);

	/*
		List of weapon which can be equiped to right hand
	*/
	UPROPERTY(ReplicatedUsing = OnRep_RightHandWeapons)
		TArray<FInventorySlot> RightHandWeapons;
	UFUNCTION()
		void OnRep_RightHandWeapons();
	/*
		Indicates whether there is some change in RightHandWeapons
	*/
	bool RightHandWeaponsUpdated;

	/* 
		[client] [server] - Add right hand weapon to RightHandWeapons
	*/
	void AddRightHandWeapon(FInventorySlot Weapon, int32 SlotID);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAddRightHandWeapon(FInventorySlot Weapon, int32 SlotID);

	/*
		[client] [server] - Remove right hand weapon from RightHandWeapons.
		Return true if something has been removed.
	*/
	bool RemoveRightHandWeapon(FName ItemID, int32 SlotID);
	/* 
		[server] - Calls RemoveRightHandWeapon On server. 
	*/
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRemoveRightHandWeapon(FName ItemID, int32 SlotID);

	/*
		Items which are equiped, like chest, gloves neckles etc. whitout assigning them to specific slot.
	*/
	UPROPERTY()
		TArray<FInventorySlot> EquippedItems;

	/* 
		Ability Inventory - Spellbook, mele action bla bla, got the point 
	**/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_AbilityInventory, Category = "Abilities")
		TArray <FAbilityInfo> AbilityInventory;
	UFUNCTION()
		void OnRep_AbilityInventory();
		bool UpdateAbilityInventory;
	/*
		Array which represents action bar. Essentialy hack, it would be better as TMap,
		but by default TMap is not exposed to reflection and replication.
	*/
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



