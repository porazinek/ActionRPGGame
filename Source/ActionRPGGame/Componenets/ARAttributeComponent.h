// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "ARAttributeBaseComponent.h"

#include "ARAttributeComponent.generated.h"

UCLASS(meta = (BlueprintSpawnableComponent), hidecategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming), MinimalAPI)
class UARAttributeComponent : public UARAttributeBaseComponent
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Attribute")
	float Health;

};



