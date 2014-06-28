// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "../Types/ARStructTypes.h"


#include "ARActionHitTrace.generated.h"

UCLASS(hidecategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming), editinlinenew, meta = (BlueprintSpawnableComponent))
class UARActionHitTrace : public UActorComponent
{
	GENERATED_UCLASS_BODY()
public:

	UFUNCTION(BlueprintPure, Category = "AR|Trace")
		FVector GetCameraAim(APawn* UsedBy);

	UFUNCTION(BlueprintPure, Category = "AR|Trace")
		FVector GetCameraDamageStartLocation(const FVector& AimDir, APawn* UsedBy);

	UFUNCTION(BlueprintPure, Category = "AR|Trace")
		USkeletalMeshComponent* GetWeaponMesh(APawn* InitiatedBy);

	UFUNCTION(BlueprintPure, Category = "AR|Trace")
		FVector GetStartLocation(FName SocketName, APawn* InitiatedBy);

	UFUNCTION(BlueprintPure, Category = "AR|Trace")
		FHitResult RangedTrace(const FVector& StartTrace, const FVector& EndTrace, APawn* InitiatedBy, TEnumAsByte<EARTraceType> TraceType);

	UFUNCTION(BlueprintCallable, Category = "AR|Trace")
		FHitResult GetHitResult(float Range, FName StartSocket, APawn* InitiatedBy, bool DrawDebug, bool UseStartSocket, TEnumAsByte<EARTraceType> TraceType);
};



