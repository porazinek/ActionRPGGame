// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "../Types/ARStructTypes.h"
#include "../Types/AREnumTypes.h"
#include "ARFXHelpersStatics.generated.h"

UCLASS()
class UARFXHelpersStatics : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
public:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, BlueprintAuthorityOnly, Category = "FX|Statics")
		void SpawnTrailEffect(UParticleSystem* trailFX, float trailSpeed, FName trailSpeedParam, FHitResult target, FName SocketName, APawn* Causer);

	UFUNCTION(BlueprintCallable, Category = "FX|Statics")
		static void AttachEffectToTarget(UParticleSystem* FXIn, FHitResult Target, FName AttachSocket, APawn* Causer);
};



