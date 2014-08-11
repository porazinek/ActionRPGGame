// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "AREnumTypes.h"
#include "ARAttributeTypes.h"
#include "Slate.h"
#include "ARItemTypes.generated.h"

USTRUCT(BlueprintType)
struct FItemAttribute : public FAttribute
{
	GENERATED_USTRUCT_BODY()
public:
	//special indicator that tells if multiple attributes of the same
	//name/type should stack
	//if false only the highest attribute from all equiped items will be used.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		bool IsStackable;

	inline bool operator!= (const FItemAttribute& Other) const
	{
		return AttributeName != Other.AttributeName
			&& ModValue != Other.ModValue
			&& IsStackable != Other.IsStackable;
	};

	FItemAttribute& operator=(const FItemAttribute& Other)
	{
		if (*this != Other)
		{
			AttributeName = Other.AttributeName;
			ModValue = Other.ModValue;
			IsStackable = Other.IsStackable;
		}
		return *this;
	};

	FItemAttribute()
		//: AttributeName(NAME_None)
		//, ModValue(0)
		: IsStackable(false)
	{};
};

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		int32 Durability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		int32 Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		int32 Value;
	/*
	Item attributes. Like Magic, Strenght, Agility etc.
	Can be additive or aubtractive.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemAttributes)
		TArray<FItemAttribute> Attributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemEffects)
		TArray<TSubclassOf<class UAREffectType>> ItemEffects;

	/*
		Default class to spawn this item. Usefull for weapons, or special active items
		which have logic different from standard AARItem.
	*/


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		USkeletalMesh* Mesh;

	FItemData()
		: Durability(0)
		, Weight(0)
		, Value(0)
		, Attributes()
		, ItemEffects()
		, Mesh(nullptr)
	{
		//	FItemAttribute ia;
		//	Attributes.Add(ia);
		//	ItemEffects.AddZeroed(1);
	};

};

USTRUCT(BlueprintType)
struct FARItemInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
		FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
		FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
		FSlateBrush ItemIcon;
		//TAssetPtr<class USlateBrushAsset> ItemIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemEffects)
		TSubclassOf<class AARItem> DefaultClass;

	UPROPERTY(EditAnywhere, Category = "Item Data")
		FItemData ItemInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
		TEnumAsByte<EItemSlot> ItemSlot;

	//inline bool operator== (const FARItemInfo& Other) const
	//{
	//	return ItemName != Other.ItemName;
	//};

	//FARItemInfo& operator=(const FARItemInfo& Other)
	//{
	//	if (*this != Other)
	//	{
	//		ItemName = Other.ItemName;
	//		ItemIcon = Other.ItemIcon;
	//		ItemSlot = Other.ItemSlot;
	//	}
	//	return *this;
	//};

	FARItemInfo()
		: ItemName("Default")
		, ItemDescription(FText::FromString("No Description"))
		, ItemSlot(EItemSlot::Item_Blank)
		, ItemIcon()
		, DefaultClass()
		, ItemInfo()
	{};

	FARItemInfo(const FARItemInfo& Other)
	{
		ItemName = Other.ItemName;
		ItemDescription = Other.ItemDescription;
		ItemSlot = Other.ItemSlot;
		ItemIcon = Other.ItemIcon;
		ItemInfo = Other.ItemInfo;
		DefaultClass = Other.DefaultClass;
	};
};

USTRUCT(BlueprintType)
struct FARItemPickupCont
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Data Asset")
		class UARItemsData* ItemData;

	UPROPERTY(VisibleAnywhere, Category = "Asset Name")
		FString ItemDataName;

	UPROPERTY(EditAnywhere, Category = "Items To Pick")
		TArray<int32> ItemsList;

	inline bool operator==(const FARItemPickupCont& Other) const
	{
		return ItemData == Other.ItemData;
	}
	FARItemPickupCont()
		: ItemsList()
	{
		ItemData = nullptr;
		ItemDataName = FString("NoName");
	};
};

USTRUCT(BlueprintType)
struct ACTIONRPGGAME_API FARItemPickerContainer
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Items To Pick")
		TArray<FARItemPickupCont> ListOfItems;

public:
	void RemoveItemContainer(FARItemPickupCont* ToRemove);

	FARItemPickerContainer()
		: ListOfItems()
	{};
};
/*
It's struct containing info about item.
It have advantage of being simple to store in single DataAsset, but Items can't
be directly placed on level.

On other hand managing all the possible actors is not better..
*/
