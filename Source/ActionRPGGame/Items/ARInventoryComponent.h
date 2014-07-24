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

	UPROPERTY(ReplicatedUsing=OnRep_InventoryChanged, BlueprintReadWrite, Category="Inventory")
		TArray<FInventorySlot> Inventory;

	UPROPERTY(Replicated)
		bool IsInventoryChanged;
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
};



