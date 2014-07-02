// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "../Types/ARStructTypes.h"


#include "ARFXEffectComponent.generated.h"

UCLASS(hidecategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming), editinlinenew, meta = (BlueprintSpawnableComponent))
class UARFXEffectComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Presitent Effect")
		UParticleSystem* PresitentFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Presitent Effect")
		FName SocketToAttach;
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_SpawnParticleEffect, Category = "Presitent Effect")
		bool IsHit;
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_SpawnParticleEffect, Category = "Presitent Effect")
		AActor* ParticleTarget;

	UFUNCTION()
		void OnRep_SpawnParticleEffect();

	UPROPERTY(Replicated)
		bool blank;
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Ability|FX")
		void SpawnTrailEffect(UParticleSystem* trailFX, float trailSpeed, FName trailSpeedParam, FHitResult target, FName SocketName, APawn* Causer);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "FX|Statics")
		void AttachEffectToTarget(UParticleSystem* FXIn, FHitResult Target, FName AttachSocket, APawn* Causer);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "FX|Statics")
		void SpawnEffectOnHitLoc(UParticleSystem* FXIn, FHitResult HitLocation, APawn* Causer);
};



