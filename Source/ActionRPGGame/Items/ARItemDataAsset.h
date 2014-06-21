// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "../Types/ARStructTypes.h"
#include "../Types/ARInvItem.h"

#include "ARItemDataAsset.generated.h"

UCLASS()
class UARItemDataAsset : public UDataAsset
{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Item")
		TArray<FARItemInfo> Items;

	UPROPERTY(EditAnywhere, Category = "Item")
		TArray<UARInvItem*> ItemsData;

public:
	UARItemDataAsset(UARItemDataAsset&);
	UARItemDataAsset();
};



