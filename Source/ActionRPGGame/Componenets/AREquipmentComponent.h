// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../Types/ARStructTypes.h"
#include "../Types/AREnumTypes.h"

#include "AREquipmentComponent.generated.h"

/*
	I'm not sure if Equipment Manager on it's own is that good idea honestly.

	It have it merits, like it can be attached to any actor, but it honestly is pretty tighly couple to
	whatever actor it is attached.

	It probably make only sense on player controller in the end, as character doesn't really loose
	equipment when for example enter vechile.
*/

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDMDOnRightWeaponActive, AARWeapon*, Weapon);

UCLASS(hidecategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming), meta = (BlueprintSpawnableComponent), DefaultToInstanced, BlueprintType) //, Within = ARPlayerController
class UAREquipmentComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()
public:

	UPROPERTY(Replicated)
	class AARCharacter* TargetCharacter;
	UPROPERTY(Replicated)
	class AARPlayerController* TargetController;

	virtual void InitializeComponent() override;
	virtual void BeginDestroy() override;
	//this is placeholder. inventory should use different data struct
	//to componesante for myriads of possible item types, not just armor in this case.
	//It doesn't even allow to keep weapons!
	//Hmm or maybe... change it to inventory of Ints! or FNames, and then just query DataAsset for whatever
	//we need!

	/* Inventory Handling **/

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_Inventory, Category = "Inventory")
		TArray<FARItemInfo> Inventory;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
		TArray<class AARItem*> EquippedItems;

	//UPROPERTY(BlueprintReadOnly, Replicated, Category = "Inventory")
	//	TArray<FARItemInfo> EquippedItems;

	int8 MaxEquippedItems;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void AddItemToInventory(FInventorySlot NewItem);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAddItemToInventory(FInventorySlot NewItem);
	void AddItemToInventoryRep();

	void PickupItem();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerPickupItem();

	UFUNCTION()
		TArray<FARItemInfo> GetItems();

	UFUNCTION()
		void OnRep_Inventory();

	void SortEquipedItemsByAttribute(FName AttributeName);

	/* Weapon Handling **/
	/**
		These weapons are equiped in character sheet,
		but are not active. Player can choose from these weapons in combat.
		To make these weapons active.
	*/
	UPROPERTY() //not sure but I think we shouldn't really replicate this back.
		TArray<FInventorySlot> LeftHandWeapons;

	UPROPERTY()
		TArray<FInventorySlot> RightHandWeapons;

	int32 MaxWeapons;

	/*
		I could probably handle it in Array. For example:
		Index = 0 Right hand weapon.
		Index = 1 Left Hand weapon.

		It would cut off half of the code.
	*/

	/*
		Curremt;y equiped weapons ready to use with input.
		Again. TMap would best for it. Indexes are not really reliable, unlike Keys in TMap.
	*/
	UPROPERTY()
	TArray<class AARWeapon*> ActiveWeapons;

	/* [Server][Client] - Add weapon to equiped weapons list**/
	UFUNCTION()
		void AddLeftHandWeapon(FName ItemName);
	/* [Server] - Add weapon to equiped weapons list **/
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAddLeftHandWeapon(FName ItemName);
	int32 MaxEquipedWeapons;

	/*
		Currently equiped weapon. It can be used to perform actions.
	*/
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_AtiveLeftHandWeapon, Category = "Active Weapons")
	class AARWeapon* ActiveLeftHandWeapon;
	/*
		Helper struct, to retrieve information form data table.
	*/
	UPROPERTY()
		FInventorySlot ActiveLeftHandWeaponStruct;
	/*
		Socket to which left hand weapon will be attached.
	*/
	UPROPERTY(EditAnywhere, Category = "Weapon")
		FName LeftWeaponSocket;
	UFUNCTION()
		void OnRep_AtiveLeftHandWeapon();
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSwapLeftWeapon();
	/*
		Swap left hand weapon from list of equiped weapons in
		PlayerController->LeftHandWeapons.
	*/
	void SwapLeftWeapon();
	/*
		Set weapon from LeftHandWeapons to ActiveLeftHandWeapon
	*/
	void SetLeftWeapon(FInventorySlot Weapon, class AARWeapon* PrevWeapon);

	/*
		UnEquip current ActiveLeftHandWeapon making it impossible to use, until it is equiped again.
	*/
	void UnEquipLeftHandWeapon(FName ItemID);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerUnEquipLeftHandWeapon(FName ItemID);

	//Right Hand Weapon
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_ActiveRightHandWeapon, Category="Active Weapons")
	class AARWeapon* ActiveRightHandWeapon;
	UPROPERTY()
		FInventorySlot ActiveRightHandWeaponStruct;
	UPROPERTY(EditAnywhere, Category = "Weapon")
		FName RightWeaponSocket;
	UFUNCTION()
		void OnRep_ActiveRightHandWeapon();
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSwapRightWeapon();
	void SwapRightWeapon();
	void SetRightWeapon(FInventorySlot Weapon, class AARWeapon* PrevWeapon);

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Weapon")
		FDMDOnRightWeaponActive OnRightWeaponActive;

	/*
	UnEquip current ActiveLeftHandWeapon making it impossible to use, until it is equiped again.
	*/
	void UnEquipRightHandWeapon(FName ItemID);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerUnEquipRightHandWeapon(FName ItemID);

	//Weapon general
	void SetAttachWeapon(class AARWeapon* Weapon, FName SocketName);

	/* Other Equipment Handling **/

	/**[Server] - central function to change item */
	UFUNCTION(BlueprintCallable, Category = "Equipment")
		void ChangeItem(FInventorySlot ItemIn, int32 OldItemSlotID);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerChangeItem(FInventorySlot ItemIn, int32 OldItemSlotID);

	void UnEquipItem(FInventorySlot ItemIn);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerUnEquipItem(FInventorySlot ItemIn);

	UPROPERTY(ReplicatedUsing = OnRep_ChestItem)
		FInventorySlot ChestItem;

	UFUNCTION()
		void OnRep_ChestItem();
	void SetChestMesh(TAssetPtr<USkeletalMesh> MeshToSet);

	bool ChangeChestItem(FInventorySlot ItemIn);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerChangeChestItem(FInventorySlot ItemIn);

	void DoAsyncChestChange();
	FStringAssetReference ChestMeshToLoad;

	void ChangeLegItem(FName ItemName);
	void DoAsyncLegChange();
	FStringAssetReference LegMeshToLoad;

};



