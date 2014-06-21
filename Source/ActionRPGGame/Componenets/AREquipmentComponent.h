// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../Types/ARStructTypes.h"

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

		virtual void InitializeComponent() OVERRIDE;
	virtual void BeginDestroy() OVERRIDE;
	//this is placeholder. inventory should use different data struct
	//to componesante for myriads of possible item types, not just armor in this case.
	//It doesn't even allow to keep weapons!
	//Hmm or maybe... change it to inventory of Ints! or FNames, and then just query DataAsset for whatever
	//we need!
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_Inventory, Category = "Inventory")
		TArray<FARItemInfo> Inventory;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
		TArray<class UARInvItem*> InventoryData;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void AddItemToInventory(FName ItemName);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAddItemToInventory(FName ItemName);
	void AddItemToInventoryRep();

	void PickupItem();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerPickupItem();

	UFUNCTION()
		TArray<FARItemInfo> GetItems();

	UFUNCTION()
		void OnRep_Inventory();

	UPROPERTY(EditAnywhere, Category = "Main Hand")
		TSubclassOf<class AARWeapon> MainHandType;
	UPROPERTY(ReplicatedUsing = OnRep_MainHand)
	class AARWeapon* MainHand;

	/* Weapon Handling **/
	/**
		These weapons are equiped in character sheet,
		but are not active. Player can choose from these weapons in combat.
		To make these weapons active.
	*/
	UPROPERTY() //not sure but I think we shouldn't really replicate this back.
		TArray<AARWeapon*> LeftHandWeapons;
	TArray<FARItemInfo> LeftHandWeaponsShared;

	UPROPERTY()
		TArray<AARWeapon*> RightHandWeapons;

	/* [Server][Client] - Add weapon to equiped weapons list**/
	UFUNCTION()
		void AddLeftHandWeapon(FName ItemName);
	/* [Server] - Add weapon to equiped weapons list **/
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAddLeftHandWeapon(FName ItemName);
	/*
		These weapons are active and can be used by Player.
	**/
	UPROPERTY(ReplicatedUsing = OnRep_MainHand)
		AARWeapon* ActiveLeftHandWeapon;
	UPROPERTY()
		FARItemInfo ActiveLeftHandWeaponStruct;
	UPROPERTY(EditAnywhere, Category = "Weapon")
		FName LeftWeaponSocket;
	UPROPERTY()
		AARWeapon* ActiveRightHandWeapon;

	int32 MaxEquipedWeapons;

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSwapLeftWeapon();

	void SwapLeftWeapon();

	void EquipLeftWeapon(FARItemInfo Weapon);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerEquipLeftWeapon(const FARItemInfo Weapon);

	void SetLeftWeapon(FARItemInfo Weapon);

	UPROPERTY()
	class AARWeapon* RightHand;
	UPROPERTY()
	class AARWeapon* LeftHand;

	FName WeaponName;

	UFUNCTION()
		void OnRep_MainHand();
	UPROPERTY(EditAnywhere, Category = "Weapon")
		FName MainHandSocket;

	UPROPERTY(Replicated)
	class AARWeapon* OffHand;

	UFUNCTION(BlueprintCallable, Category = "Equipment")
		void EquipWeapon(TSubclassOf<class AARWeapon> Weapon, FName SocketName, FName ItemName);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerEquipWeapon(TSubclassOf<class AARWeapon> Weapon, FName SocketName, FName ItemName);

	void SetAttachWeapon(class AARWeapon* Weapon, FName SocketName);
	UFUNCTION(BlueprintCallable, Category = "Equipment")
		void ChangeItem(FName ItemName);

	//UFUNCTION(BlueprintCallable, Category = "Equipment")
	//	void ChangeItem(FARItemInfo ItemIn);

	UPROPERTY(ReplicatedUsing = OnRep_ChestItem)
		FName ChestItem;
	//FARItemInfo ChestItem;

	UFUNCTION()
		void OnRep_ChestItem();
	void SetChestMesh(TAssetPtr<USkeletalMesh> MeshToSet);

	FString ItemDataAssetPath;

	FString HeadItemDataAssetPath;

	FString ShoulderItemDataAssetPath;

	FString ChestItemDataAssetPath;

	FString HandsItemDataAssetPath;

	FString LegItemDataAssetPath;

	FString FootItemDataAssetPath;

	FString WeaponItemDataAssetPath;

	/*
	Special item that exist only as data in DataAsset. They may or may not affect owning character
	but they are only visible as part of HUD information.
	While other items consist of object displayed in game world as well as item that can respond
	to events or input.
	*/
	FString BlankItemDataAssetPath; 

	/**[Server] - central function to change item */
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerChangeItem(FName ItemName);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerChangeChestItem(FName ItemName);

	void ChangeItemValid(FName ItemName);

	void ChangeChestItem(FName ItemName);

	FStringAssetReference ChestMeshToLoad;

	void DoAsyncChestChange();
};



