// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "../Types/ARStructTypes.h"

#include "../Types/ARAttributeTypes.h"
#include "ARBeamCue.generated.h"

UCLASS(hidecategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming), editinlinenew, meta = (BlueprintSpawnableComponent))
class UARBeamCue : public UActorComponent
{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Beam Effect")
		UParticleSystem* TrailFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Beam Effect")
		FName StartSocket;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Beam Effect")
		FName OriginParam;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Beam Effect")
		FName ImpactParam;
	UPROPERTY(ReplicatedUsing = OnRep_Hit, BlueprintReadWrite, Category="Hit Info")
		FHitInfo HitInfo;
	UFUNCTION()
		void OnRep_Hit();
	void SimulateHitOnClients(FVector Origin, FVector Location, FName StartSocket);

	UFUNCTION(BlueprintCallable, Category = "Effects")
		void PlayEffectInSinglePlayer();
};



