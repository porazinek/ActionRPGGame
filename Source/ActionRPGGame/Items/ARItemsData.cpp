// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"
#include "ARItemsData.h"

UARItemsData::UARItemsData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UARItemsData::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);
	//if (Ar.IsLoading() && (Ar.UE4Ver() < VER_UE4_ADD_TRANSACTIONAL_TO_DATA_ASSETS))
	//{
	//	SetFlags(RF_Transactional);
	//}
	//if (Ar.IsLoading())
	//{
	//	int32 ObjectNum;
	//	Ar << ObjectNum;

	//	for (int32 ObjIdx = 0; ObjIdx < ObjectNum; ObjIdx++)
	//	{
	//		FName KeyName;
	//		Ar << KeyName;

	//		FARItemInfo Data;
	//		Ar << Data;

	//		ItemMap.Add(KeyName, Data);
	//	}
	//}
	//else if (Ar.IsSaving())
	//{
	//	int32 ObjectNum = ItemMap.Num();
	//	Ar << ObjectNum;
	//	if (ItemMap.Num() > 0)
	//	{
	//		for (auto It = ItemMap.CreateIterator(); It; ++It)
	//		{
	//			FName KeyName = It.Key();
	//			Ar << KeyName;

	//			FARItemInfo Data = It.Value();
	//			Ar << Data;
	//		}
	//	}
	//}
}

void UARItemsData::PostLoad()
{
	Super::PostLoad();
	//int32 Index = 0;
	//for (auto It = ItemMap.CreateIterator(); It; ++It)
	//{
	//	FItemEntry ItemEntry;
	//	ItemEntry.Index = Index;
	//	ItemEntry.Key = It.Key();
	//	ItemEntry.ItemDataInfo = It.Value();
	//	EditEntries.Add(ItemEntry);
	//	Index++;
	//}
}

void UARItemsData::AddItem(const FARItemInfo& Data)
{
	FName Key = FName(*FGuid::NewGuid().ToString());
	//ItemMap.Add(Key, Data);
	//int32 Index = ItemMap.Num() - 1;
	//FItemEntry ItemEntry;
	//ItemEntry.Index = Index;
	//ItemEntry.Key = Key;
	//ItemEntry.ItemDataInfo = Data;
	int32 ArrIdx = EditEntries.Num();
	//if (ArrIdx < 0)
	//	ArrIdx = 0;

	FItemEntry testEntry;

	testEntry.Index = ArrIdx;
	testEntry.Key = Key;
	testEntry.ItemDataInfo = Data;

	EditEntries.Add(testEntry);
	//add new item to the last slot in array;
	

	//EditEntries.Add(ItemEntry);
	MarkPackageDirty();
}

void UARItemsData::AddNewItem()
{
	FName Key = FName(*FGuid::NewGuid().ToString());
	FARItemInfo EmptyData;
	EmptyData.ItemName = "New Item";
	ItemMap.Add(Key, EmptyData);
	
	
	//add new item to the last slot in array;
	int32 Index = ItemMap.Num() - 1;
	FItemEntry ItemEntry;
	ItemEntry.Index = Index;
	ItemEntry.Key = Key;
	ItemEntry.ItemDataInfo = EmptyData;
	EditEntries.Add(ItemEntry);
	MarkPackageDirty();
}

void UARItemsData::DeleteItem(const FName& Key)
{

}

FItemEntry UARItemsData::GetItemEntry(FName Key)
{
	FARItemInfo* tempData = ItemMap.Find(Key);
	FItemEntry itemEntry;
	if (tempData)
	{
		itemEntry.ItemDataInfo = *tempData;
		itemEntry.Key = Key;
		return itemEntry;
	}

	return itemEntry;
}

void UARItemsData::SetCurrentEntry(const FName& Key)
{
	RebuildEditEntries();
}

//void UARItemsData::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
//{
//	Super::PostEditChangeProperty(PropertyChangedEvent);
//}

/*
	Should be more functions which handle refreshing list.
	1. While adding, we don't need to refresh everything, because we add to end of list.
	2. While deleting, we will need to refresh and rebuild everything, because it will invalidate indexes.
	3. While editing we shouldn't do anything. Just update array, and map, and refresh Ui instead.
*/
void UARItemsData::RebuildEditEntries()
{
	EditEntries.Empty(EditEntries.Num());
	int32 Index = 0;
	for (auto It = ItemMap.CreateIterator(); It; ++It)
	{
		FItemEntry ItemEntry;
		ItemEntry.Index = Index;
		ItemEntry.Key = It.Key();
		ItemEntry.ItemDataInfo = It.Value();
		EditEntries.Add(ItemEntry);
		Index++;
	}
}

void UARItemsData::SetItemInMap(int32 IndexIn)
{
	//for (FItemEntry& item : EditEntries)
	//{
	//	ItemMap.FindChecked(item.Key) = item.DataTest;
	//}
	//if (EditEntries.Num() == 0)
	//{
	//	RebuildEditEntries();
	//	return;
	//}
	//FARItemInfo& data = EditEntries[IndexIn].DataTest;
	//FItemEntry* item = ItemMap.Find(EditEntries[IndexIn].Key);
	//*item = EditEntries[IndexIn].DataTest;
	//EditEntries[IndexIn].DataTest;
	//ItemMap.FindChecked(EditEntries[IndexIn].Key) = EditEntries[IndexIn].ItemDataInfo;
	//RebuildEditEntries();
}

FItemEntry UARItemsData::GetItemFromArray(int32 Index)
{
	return EditEntries[Index];
}

FItemEntry* UARItemsData::GetItemFromArrayPtr(int32 Index)
{
	return &EditEntries[Index];
}

FItemEntry& UARItemsData::GetItemFromArrayRef(int32 Index)
{
	return EditEntries[Index];
}

FARItemInfo UARItemsData::GetItemDataFromArray(int32 Index)
{
	return EditEntries[Index].ItemDataInfo;
}

FARItemInfo* UARItemsData::GetItemDataFromArrayPtr(int32 Index)
{
	return &EditEntries[Index].ItemDataInfo;
}

FARItemInfo* UARItemsData::GetItemPtr(const FName& Key)
{
	return ItemMap.Find(Key);
}

FARItemInfo& UARItemsData::GetItemRef(const FName& Key)
{
	return ItemMap.FindChecked(Key);
}