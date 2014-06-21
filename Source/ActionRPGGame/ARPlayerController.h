// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "Types/ARStructTypes.h"

#include "Types/ARInvItem.h"

#include "ARPlayerController.generated.h"

UCLASS()
class AARPlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()
public:
	//virtual void PostInitializeComponents() OVERRIDE;

	UPROPERTY(ReplicatedUsing=OnRep_InventoryChanged)
		TArray<FARItemInfo> Inventory;

	UPROPERTY(ReplicatedUsing = OnRep_InventoryChanged)
		TArray<UObject*> InventoryObj;

	UPROPERTY(ReplicatedUsing = OnRep_InventoryChanged)
		TArray<FInventorySlot> InventorySmall;

		TMap<int32, FInventorySlot> InventoryMap;

	//TWeakPtr<FARItemInfo> weakitem;
	//max inventory size. Can be increased in game. Probably should change to int8
	//make it protected or private, and access it trough accessor in other classes.
	UPROPERTY(Replicated)
		int32 MaxInventorySize;

	UFUNCTION()
		void OnRep_InventoryChanged();

	void AddItemToInventory(FARItemInfo& Item);
	void SwapItemPosition(FARItemInfo& Item, int32 NewIndex);
	bool IsInventoryChanged;
};



