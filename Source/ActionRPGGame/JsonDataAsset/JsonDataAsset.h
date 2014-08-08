// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "Serialization/Json/Json.h"
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "JsonDataAsset.generated.h"

/*
	Base struct for imported json object.

	TArray<FOtherData> OtherData;
	}

	USTRUCT()
	struct FMyJsonContainer : public FJsonDataObject
	{
		GENERATED_USTRUCT_BODY()
	public:
		UPROPERTY()
			FString MyString;
	};

	Coresponding Json file would look like that:

	{
		"MyData": { - arbitrary key name for FMyJsonContainer
			"MyString" : "Some string" -- everything here must match structure of FMyJsonCtainer 1:1
		},
		"OtherData": {
			"MyString" : "Other string"
		}
	}

	That at least in theory.
*/


USTRUCT()
struct FJsonDataObject
{
	GENERATED_USTRUCT_BODY()
};

USTRUCT()
struct FMyJsonContainer : public FJsonDataObject
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
		FString MyString;


};

UCLASS()
class ACTIONRPGGAME_API UJsonDataAsset : public UObject
{
	GENERATED_UCLASS_BODY()
public:
	//UPROPERTY()
	TSharedPtr<FJsonObject> ImportedJsonObject;

	UPROPERTY()
		FMyJsonContainer JsonObjectStruct;

	UPROPERTY()
		FString ImportPath;

	//UPROPERTY()
	//	FJsonDataObject Awesome;

	TMap<FName, FMyJsonContainer> ObjectMap;
	
	virtual void Serialize(FArchive& Ar) override;

	void SetItemMap();

	FMyJsonContainer* FindStruct(const FName& KeyName)
	{
		if (ObjectMap.Num() > 0)
		{
			return ObjectMap.Find(KeyName);
		}
		return nullptr;
	}
};



