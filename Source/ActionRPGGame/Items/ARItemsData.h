// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../Types/ARAttributeTypes.h"
#include "Serialization/Json/Json.h"
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "ARItemsData.generated.h"

/*
	Custom asset, which contain information about ARItem. It exist only because there no
	direct access to TMap in editor, and this entire thing is wrapper around it.

	There are also will be simple editor which will allow editing information
	about item. Things like:
	1. Icon.
	2. Attributes.
	3. Stats.
	4. Description.
	

	Information is stored in TMap, because of fast lookups. This asset is going to be very intensly used
	for displaying information in UI, creating loot tables for pickup items/NPC/monsters/etc.
*/



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
		TAssetPtr<class USlateBrushAsset> ItemIcon;
		//FSlateBrush ItemIcon;

	UPROPERTY(EditAnywhere, Category = "Item Data")
		FItemData ItemInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
		TEnumAsByte<EItemSlot> ItemSlot;

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
		, ItemInfo()
	{};
};

USTRUCT(meta = (DisplayName = "Items Data"))
struct FDataTestStruct
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Struct")
		FString TestString;
};

/*
	For editing.
	Add int32 Index, for direct access for items in array, while editing.
*/
USTRUCT()
struct FItemEntry
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
		FName Key;
	UPROPERTY()
		int32 Index;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
		FARItemInfo ItemDataInfo;
};

UCLASS()
class ACTIONRPGGAME_API UARItemsData : public UObject
{
	GENERATED_UCLASS_BODY()
public:
	virtual void Serialize(FArchive& Ar) override;

	TMap<FName, FARItemInfo> ItemMap;

	FARItemInfo* GetItemData(const FName& Key);

	FARItemInfo& GetItemRef(const FName& Key);

	virtual void PostLoad() override;

#if WITH_EDITOR
	/*
		Helper array to make editing TMap in editor easier.

		I could probabaly just edit TMap directly. But then I would have to create entire
		widget for it, by compying data to Array, I can let PropertyEditor, generate everything for me.

		If you read this and have better idea, than this, please tell!
	*/
	UPROPERTY(EditAnywhere, Transient, Category = ItemData)
		TArray<FItemEntry> EditEntries;

	/*
		Add Item with Specific data.
	*/
	void AddItem(const FARItemInfo& Data);

	/*
		Add Default Item
	*/
	void AddNewItem();

	/*
		Deleting is very heavy operation. Because after each deleted item, we need to reconstruct
		entire EditEntries array from scartch, to be 100% sure, it have corret indexes and order.
	*/
	/*
		Delete single item.
	*/
	void DeleteItem(const FName& Key);

	void SetCurrentEntry(const FName& Key);

	void SetItemInMap(int32 IndexIn);

	FItemEntry GetItemEntry(FName Key);

	void RebuildEditEntries();

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};



