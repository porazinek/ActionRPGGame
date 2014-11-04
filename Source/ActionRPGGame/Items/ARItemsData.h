// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../Types/ARAttributeTypes.h"
#include "../Types/ARItemTypes.h"

#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "ARItemsData.generated.h"

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

UCLASS(meta = (DisplayName = "Items Data"), BlueprintType, Blueprintable)
class ACTIONRPGGAME_API UARItemsData : public UObject
{
	GENERATED_UCLASS_BODY()
public:
	virtual void Serialize(FArchive& Ar) override;

	TMap<FName, FARItemInfo> ItemMap;

	UPROPERTY()
		TArray<FItemEntry> TestArray;

	FItemEntry GetItemFromArray(int32 Index);

	FItemEntry* GetItemFromArrayPtr(int32 Index);

	FItemEntry& GetItemFromArrayRef(int32 Index);

	FARItemInfo GetItemDataFromArray(int32 Index);

	FARItemInfo* GetItemDataFromArrayPtr(int32 Index);

	FARItemInfo* GetItemPtr(const FName& Key);

	FARItemInfo& GetItemRef(const FName& Key);

	virtual void PostLoad() override;

//#if WITH_EDITOR
	/*
		Helper array to make editing TMap in editor easier.

		I could probabaly just edit TMap directly. But then I would have to create entire
		widget for it, by compying data to Array, I can let PropertyEditor, generate everything for me.

		If you read this and have better idea, than this, please tell!
	*/
	UPROPERTY(EditAnywhere, Category = ItemData)
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

	//virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
//#endif
};



