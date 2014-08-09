// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "../Types/ARStructTypes.h"
#include "../Types/ARItemTypes.h"
#include "../Types//AREnumTypes.h"
#include "ARItem.generated.h"

/*
	Item is base class for objects that:
	
	1. Base class for items which can be attached to pawns. Like Weapons.
*/
UCLASS()
class AARItem : public AActor
{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Item Data")
		FItemData ItemInfo;


	UPROPERTY(EditAnywhere, Category = "Item")
		FName ItemName;
	UPROPERTY(EditAnywhere, Category = "Item")
		FString ItemDescription;
	UPROPERTY(EditAnywhere, Category = "Item")
		FSlateBrush ItemIcon;

	UPROPERTY(EditAnywhere, Category = "Item")
		USlateBrushAsset* ItemIconAsset;

	UPROPERTY()
	class AARCharacter* ItemOwner;

	//Set when adding to equipped items.
	UPROPERTY(Replicated)
		FName ItemID;
	UPROPERTY(Replicated)
		int32 ItemIndex;

	TEnumAsByte<EItemSlot> ItemSlotEquipped;

	UPROPERTY(EditAnywhere, Category = "Cosmetics")
		TEnumAsByte<EAttachSocketType> SocketType;

	//bool operator==(const AARItem& Other) const
	//{
	//	return ItemID == Other.ItemID;
	//};
};



