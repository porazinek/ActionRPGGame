// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "../Types/ARStructTypes.h"
#include "../Types/AREnumTypes.h"
#include "ARItem.h"

#include "ARArmor.generated.h"

UCLASS(minimalapi)
class AARArmor : public AARItem
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, Category = "Armor Mesh")
		TAssetPtr<USkeletalMesh> ArmorMesh;

	UPROPERTY(EditAnywhere, Category = "Armor")
		TEnumAsByte<EItemSlot> ItemSlot;
};



