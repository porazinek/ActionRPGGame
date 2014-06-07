// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "ARAttributeBaseComponent.generated.h"
/*
	Despite the name, AttributeBaseComponent DO NOT hold any attributes.
	Attributes should be defined in component derived from this class.

	Access to attribute properties is done trough Unreal code reflection system.
*/
UCLASS(meta = (BlueprintSpawnableComponent), hidecategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming), MinimalAPI)
class UARAttributeBaseComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

};



