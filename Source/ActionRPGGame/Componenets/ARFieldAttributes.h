// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "../Types/ARStructTypes.h"

#include "ARAttributeBaseComponent.h"
#include "ARFieldAttributes.generated.h"

/*
	Burning,
	Freezed,
	Gas,
*/

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDMDOnBurningBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDMDOnBurningEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDMDOnFreezed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDMDOnTurnedGas);

UCLASS(hidecategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming), editinlinenew, meta = (BlueprintSpawnableComponent))
class UARFieldAttributes : public UARAttributeBaseComponent
{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		float Health;
};



