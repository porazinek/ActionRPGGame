// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "../Types/ARStructTypes.h"

#include "ARInventoryComponent.generated.h"

/*
	Major refactor to inventory handling.
	Player controller is to cluttered, I'm moving everthing to separate component,
	and hopefully I will somehow be able to deal with replication ;)
*/
UCLASS(hidecategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming), editinlinenew, meta = (BlueprintSpawnableComponent))
class UARInventoryComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()
public:
	virtual void InitializeComponent() override;
	virtual void OnRegister() override;

	UPROPERTY(Replicated, EditAnywhere, Category = "Inventory")
		int8 MaxInventorySize;
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
		int32 InitialInventorySize;

	/*
		Array which contains list of all currently possesd items.
		It is not accessible on clients.
		Clients can never make any operations on it.
		It can be only used by two functions:
		AddItemToInventory
		RemoveItemFromInventory.
	*/
	UPROPERTY()
		TArray<FARDragDropInfo> PossesedItems;

	UPROPERTY(ReplicatedUsing=OnRep_InventoryChanged, BlueprintReadWrite, Category="Inventory")
		TArray<FARDragDropInfo> Inventory;

	UPROPERTY(Replicated)
		bool IsInventoryChanged;
	UFUNCTION()
		void OnRep_InventoryChanged();

	UFUNCTION(Client, Reliable)
		void ClientSetInventoryChanged();

	void AddPickItemToInventory(FARDragDropInfo Item);

	void AddItemToInventory(FARDragDropInfo Item);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAddItemToInventory(FARDragDropInfo Item);

	void AddItemToInventoryOnSlot(FARDragDropInfo Item, int32 SlotID);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAddItemToInventoryOnSlot(FARDragDropInfo Item, int32 SlotID);

	bool RemoveItemFromInventory(FName ItemID, int32 SlotID);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRemoveItemFromInventory(FName ItemID, int32 SlotID);

	/*
		This function will completly remove item, from player possession.
		Unlike RemoveItemFromInventory which is used to manage items between inventory
		and character sheet.
	*/
	void RemoveItem(FName ItemID);
};



