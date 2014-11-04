// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "IARTracing.h"
#include "ARTraceComponent.generated.h"

/*
	Component whci can be added to any actor implementing IARTracing, which provides encapsulated tracing functionality
*/
UCLASS(hidecategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming), editinlinenew, meta = (BlueprintSpawnableComponent))
class UARTraceComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "AR|Trace")
		FVector GetCameraAim();

	UFUNCTION(BlueprintPure, Category = "AR|Trace")
		FVector GetCameraDamageStartLocation(const FVector& AimDir);

	UFUNCTION(BlueprintPure, Category = "AR|Trace")
		USkeletalMeshComponent* GetWeaponMesh();

	UFUNCTION(BlueprintPure, Category = "AR|Trace")
		FVector GetStartLocation(FName SocketName, TEnumAsByte<EWeaponHand> Hand);

	/*
	Version which takes AARCharacter as argument.
	*/
	UFUNCTION(BlueprintPure, Category = "AR|Trace")
		FVector GetStartLocationFromCharacter(FName SocketName, TEnumAsByte<EWeaponHand> Hand);

	UFUNCTION(BlueprintPure, Category = "AR|Trace")
		FHitResult RangedTrace(const FVector& StartTrace, const FVector& EndTrace, TEnumAsByte<EARTraceType> TraceType);

	UFUNCTION(BlueprintCallable, Category = "AR|Trace")
		FHitResult GetHitResult(float Range, FName StartSocket, bool DrawDebug, bool UseStartSocket, TEnumAsByte<EARTraceType> TraceType, TEnumAsByte<EWeaponHand> Hand);


private:
	//no new, no delete, in case you wonder. I store pointer initalized at contruction time
	//so I don't have to do InterfaceCast<> in each function every time I use this function.
	IIARTracing* ITracing;
	UPROPERTY()
		AARCharacter* ARCharOwner;
	UPROPERTY()
		AARPlayerController* ARPCOwner;

};



