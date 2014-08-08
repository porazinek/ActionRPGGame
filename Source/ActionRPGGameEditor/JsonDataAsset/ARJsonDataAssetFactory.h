// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "Factories/Factory.h"
//#include "../../ActionRPGGame/JsonDataAsset/JsonDataAsset.h"
#include "ARJsonDataAssetFactory.generated.h"

UCLASS()
class UARJsonDataAssetFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, Category = JsonAsset)
	TSubclassOf<UJsonDataAsset> DataAssetClass;

	// UFactory interface
	//virtual bool ConfigureProperties() override;
	//virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

	virtual UObject* FactoryCreateText(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const TCHAR*& Buffer, const TCHAR* BufferEnd, FFeedbackContext* Warn) override;


	TSharedPtr<FJsonObject> ParseJSON(const FString& InData, const FString& ErrorNames);

	// End of UFactory interface
};



