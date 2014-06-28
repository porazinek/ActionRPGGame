// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "AREnumTypes.h"
#include "Slate.h"
#include "ARAttributeTypes.generated.h"

USTRUCT(BlueprintType)
struct FAttribute
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		FName AttributeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		float ModValue;

	FAttribute() {};
};

USTRUCT(BlueprintType)
struct FHitInfo
{
	GENERATED_USTRUCT_BODY()
public:

};