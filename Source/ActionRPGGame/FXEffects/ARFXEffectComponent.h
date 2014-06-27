// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "../Types/ARStructTypes.h"


#include "ARFXEffectComponent.generated.h"

UCLASS(hidecategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming), editinlinenew, meta = (BlueprintSpawnableComponent))
class UARFXEffectComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()
public:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Ability|FX")
		void SpawnTrailEffect(UParticleSystem* trailFX, float trailSpeed, FName trailSpeedParam, FHitResult target, FName SocketName, APawn* Causer);
};



