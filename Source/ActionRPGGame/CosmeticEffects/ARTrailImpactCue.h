// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "../Types/ARStructTypes.h"

#include "../Types/ARAttributeTypes.h"
#include "ARTrailImpactCue.generated.h"


/*
	TODO::
	Figure out why trail doesn't spawn, when actor is out of sight of other actors!.
*/
/*
	Spawn two particle systems:
	1. Trail
	2. Impact on hit object.
*/
UCLASS(hidecategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming), editinlinenew, meta = (BlueprintSpawnableComponent))
class UARTrailImpactCue : public UActorComponent
{
	GENERATED_UCLASS_BODY()
public:
	virtual void InitializeComponent() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trail Effect")
		UParticleSystem* TrailFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trail Effect")
		UParticleSystem* ImpactFX;

	UPROPERTY()
		UParticleSystemComponent* TrailPSC;
//	UPROPERTY()
//		UParticleSystemComponent* ImpactPSC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trail Effect")
		FName StartSocket;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trail Effect")
		float TrailSpeed2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trail Effect")
		FName TrailSpeedParam2;

	UPROPERTY(ReplicatedUsing = OnRep_Hit, RepRetry, BlueprintReadWrite, Category = "Hit Info")
		FHitInfo HitInfo;

	UFUNCTION()
		void OnRep_Hit();
	/*
		Call it in blueprint after setting up HitInfo.
		Otherwise effect will not be played for Owner or in Singleplayer.
	*/
	UFUNCTION(BlueprintCallable, Category = "Effects")
	void SimulateHitOnClients(FVector Origin, FVector Location, FName StartSocketIn);

	UFUNCTION(BlueprintCallable, Category = "Effects")
		void SpawnEffectOnOwner();
	void OnCollide(FName EventName, float EmitterTime, int32 ParticleTime, FVector Location, FVector Velocity, FVector Direction, FVector Normal, FName BoneName);
};



