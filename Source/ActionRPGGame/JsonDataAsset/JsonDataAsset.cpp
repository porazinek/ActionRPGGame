// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"
#include "JsonDataAsset.h"

UJsonDataAsset::UJsonDataAsset(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}

void UJsonDataAsset::Serialize(FArchive& Ar)
{
	//Super::Serialize(Ar);
	//if (Ar.IsLoading())
	//{
	//	int32 ObjectNum;
	//	Ar << ObjectNum;

	//	for (int32 ObjIdx = 0; ObjIdx < ObjectNum; ObjIdx++)
	//	{
	//		FName KeyName;
	//		Ar << KeyName;

	//		FMyJsonContainer Data;
	//		Ar << Data;

	//		ObjectMap.Add(KeyName, Data);
	//	}
	//}
	//else if (Ar.IsSaving())
	//{
	//	if (ObjectMap.Num() > 0)
	//	{
	//		int32 ObjectNum = ObjectMap.Num();
	//		Ar << ObjectNum;

	//		for (auto It = ObjectMap.CreateIterator(); It; ++It)
	//		{
	//			FName KeyName = It.Key();
	//			Ar << KeyName;

	//			FMyJsonContainer Data = It.Value();
	//			Ar << Data;
	//		}
	//	}
	//}
}

void UJsonDataAsset::SetItemMap()
{
	for (auto It = ImportedJsonObject->Values.CreateIterator(); It; ++It)
	{
		FName Key = FName(*It.Key());
		const TSharedPtr<FJsonObject> OutObject = ImportedJsonObject->GetObjectField(It.Key());
		FMyJsonContainer tempCon;

		FJsonObjectConverter::JsonObjectToUStruct(OutObject.ToSharedRef(), tempCon.StaticStruct(), &tempCon, 0, 0);
	
		ObjectMap.Add(Key, tempCon);
	}
}