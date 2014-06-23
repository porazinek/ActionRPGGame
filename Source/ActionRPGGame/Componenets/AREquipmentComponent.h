// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../Types/ARStructTypes.h"
#include "../Types/AREnumTypes.h"

#include "AREquipmentComponent.generated.h"

UCLASS(hidecategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming), meta = (BlueprintSpawnableComponent), DefaultToInstanced, BlueprintType) //, Within = ARPlayerController
class UAREquipmentComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()
public:

	UPROPERTY(Replicated)
	class AARCharacter* TargetCharacter;
	UPROPERTY(Replicated)
	class AARPlayerController* TargetController;

	UPROPERTY()
		bool IsInventoryChanged;

	//UDataTable* ChestItemDataTable;


	virtual void InitializeComponent() OVERRIDE;
	virtual void BeginDestroy() OVERRIDE;
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
		TArray<class AARWeapon*> LeftHandWeapons;
		TArray<FARItemInfo> LeftHandWeaponsShared;

	UPROPERTY()
		TArray<class AARWeapon*> RightHandWeapons;

	/* [Server][Client] - Add weapon to equiped weapons list**/
	UFUNCTION()
		void AddLeftHandWeapon(FName ItemName);
	/* [Server] - Add weapon to equiped weapons list **/
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAddLeftHandWeapon(FName ItemName);
	/*
		These weapons are active and can be used by Player.
	**/
	UPROPERTY(ReplicatedUsing = OnRep_AtiveLeftHandWeapon)
		class AARWeapon* ActiveLeftHandWeapon;
	UPROPERTY()
		FARItemInfo ActiveLeftHandWeaponStruct;
	UPROPERTY(EditAnywhere, Category = "Weapon")
		FName LeftWeaponSocket;
	UFUNCTION()
		void OnRep_AtiveLeftHandWeapon();

	UPROPERTY()
		class AARWeapon* ActiveRightHandWeapon;

	int32 MaxEquipedWeapons;

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSwapLeftWeapon();

	void SwapLeftWeapon();

	void EquipLeftWeapon(FARItemInfo Weapon);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerEquipLeftWeapon(const FARItemInfo Weapon);

	void SetLeftWeapon(FARItemInfo Weapon, class AARWeapon* PrevWeapon);

	void SwapRightWeapon();

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



