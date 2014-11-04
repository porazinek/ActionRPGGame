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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDMDOnLeftWeaponActive, AARWeapon*, Weapon); 

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDMDOnWeaponActive, AARWeapon*, Weapon);

UCLASS(hidecategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming), meta = (BlueprintSpawnableComponent), DefaultToInstanced, BlueprintType) //, Within = ARPlayerController
class UAREquipmentComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()
public:

	UPROPERTY(Replicated)
	class AARCharacter* ARCharacterOwner;
	UPROPERTY(Replicated)
	class AARPlayerController* ARPCOwner;
	UPROPERTY(Replicated)
	class UARInventoryComponent* Inventory;

	virtual void InitializeComponent() override;
	virtual void BeginDestroy() override;

	UPROPERTY(Replicated)
		TArray<FARDragDropInfo> EquipmentSlots;

	/*
		Consolidate all Left/Right Hand function.
		0 - left hand 
		1 - right Hand
		0  1  - makes sense right ?
	*/
	/* Inventory Handling **/
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
		TArray<class AARItem*> EquippedItems;

	void EquipItem(FARDragDropInfo ItemIn);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerEquipItem(FARDragDropInfo ItemIn);

	UPROPERTY(ReplicatedUsing = OnRep_ChestItem)
		FARDragDropInfo ChestItem;

	UFUNCTION()
		void OnRep_ChestItem();
	void SetChestMesh(TAssetPtr<USkeletalMesh> MeshToSet);

	bool ChangeChestItem(FARDragDropInfo ItemIn);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerChangeChestItem(FARDragDropInfo ItemIn);

	void DoAsyncChestChange();
	FStringAssetReference ChestMeshToLoad;

	void ChangeLegItem(FName ItemName);
	void DoAsyncLegChange();
	FStringAssetReference LegMeshToLoad;


	int8 MaxEquippedItems;
	
	/*
		To consolidate it into single array,
		I would need new struct which would indicate which slot is left hand and which slot if right hand.
		It would make it harder to display, and I'm not sure if that is worth the effort honestly.
	*/
	/*
		List of sockets to which weapon might be attached. After equiping it, but while it is not active.
	*/
	UPROPERTY(EditAnywhere, Category = "Cosmetics")
	TArray<FARAttachmentSocket> WeaponSockets;

	UPROPERTY(ReplicatedUsing=OnRep_LeftHandWeapons) //not sure but I think we shouldn't really replicate this back.
		TArray<FARDragDropInfo> LeftHandWeapons;
	/*
		List of equiped weapons. They are not active but can be quickly switched.
	*/
	TArray<AARWeapon*> EquipedWeapons;

	/*
		Change it to NetMulticast and push it directly without Repnotify ?
	*/
	void AttacheSheathedWeapon(TArray<FARDragDropInfo> WeaponsIn, TArray<FARAttachmentSocket> WeaponSocketsIn, int32 HandIn);
	
	UFUNCTION(NetMulticast, Reliable)
		void MulticastAttacheSheathedWeapon(FARDragDropInfo WeaponsIn, int32 HandIn);
	UFUNCTION(NetMulticast, Reliable)
		void AttachSheathhWeaponOnSwap(class AARWeapon* LastWeapon, const TArray<FARAttachmentSocket>& WeaponSocketsIn, int32 HandIn);
	UFUNCTION(NetMulticast, Reliable)
		void MulticastDetachWeaponSlotSwap(FName WeaponID, int32 HandIn);
	
	void DetachSheathedWeapon(class AARWeapon* WeaponToDetach);
	
	void AddWeapon(FARDragDropInfo Weapon, int32 SlotID, int32 Hand);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAddWeapon(FARDragDropInfo Weapon, int32 SlotID, int32 Hand);

	bool RemoveWeapon(FName Weapon, int32 SlotID, int32 Hand);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRemoveWeapon(FName Weapon, int32 SlotID, int32 Hand);

	UFUNCTION()
		void OnRep_LeftHandWeapons();
	/*
		Indicates whether there is some change in LeftHandWeapons
	*/
	bool LeftHandWeaponsUpdated;

	UPROPERTY(ReplicatedUsing = OnRep_RightHandWeapons)
		TArray<FARDragDropInfo> RightHandWeapons;
	UFUNCTION()
		void OnRep_RightHandWeapons();
	/*
		Indicates whether there is some change in RightHandWeapons
	*/
	bool RightHandWeaponsUpdated;

	/* Weapon Handling **/
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


	void SwapWeapon(int32 Hand);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSwapWeapon(int32 Hand);

	void SetWeapon(FARDragDropInfo Weapon, class AARWeapon* PrevWeapon, int32 Hand);
	void UnEquipWeapon(FName ItemID, int32 Hand);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerUnEquipWeapon(FName ItemID, int32 Hand);
	UPROPERTY()
	TArray<class AARWeapon*> ActiveWeapons;

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
		FARDragDropInfo ActiveLeftHandWeaponStruct;

	/*
		Socket to which left hand weapon will be attached.
	*/
	UPROPERTY(EditAnywhere, Category = "Weapon")
		FName LeftWeaponSocket;
	UFUNCTION()
		void OnRep_AtiveLeftHandWeapon();

	//Right Hand Weapon
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_ActiveRightHandWeapon, Category="Active Weapons")
	class AARWeapon* ActiveRightHandWeapon;
	UPROPERTY()
		FARDragDropInfo ActiveRightHandWeaponStruct;
	UPROPERTY(EditAnywhere, Category = "Weapon")
		FName RightWeaponSocket;
	UFUNCTION()
		void OnRep_ActiveRightHandWeapon();

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Weapon")
		FDMDOnRightWeaponActive OnRightWeaponActive;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Weapon")
		FDMDOnLeftWeaponActive OnLeftWeaponActive;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Weapon")
		FDMDOnWeaponActive OnWeaponActive;
	//Weapon general
	void SetAttachWeapon(class AARWeapon* Weapon, FName SocketName);

	void SetSeathedWeapon(class AARWeapon* WeaponIn, FName SocketNameIn);

	/* Other Equipment Handling **/

	/**[Server] - central function to change item */
	UFUNCTION(BlueprintCallable, Category = "Equipment")
		void ChangeItem(FARDragDropInfo ItemIn, int32 OldItemSlotID);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerChangeItem(FARDragDropInfo ItemIn, int32 OldItemSlotID);

	void UnEquipItem(FARDragDropInfo ItemIn);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerUnEquipItem(FARDragDropInfo ItemIn);



	/*
		Helper functions.
	*/
	void SortEquipedItemsByAttribute(FName AttributeName);
};



