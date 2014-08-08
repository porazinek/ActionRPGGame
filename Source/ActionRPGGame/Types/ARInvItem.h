// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "AREnumTypes.h"

#include "ARInvItem.generated.h"

UCLASS()
class UARInvItem : public UDataAsset
{
	GENERATED_UCLASS_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
		int32 ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
		FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
		TAssetPtr<UTexture2D> ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
		TEnumAsByte<EItemSlot> ItemSlot;

	UPROPERTY(EditAnywhere, Category = "Item")
		TAssetPtr<USkeletalMesh> ItemMesh;

};



