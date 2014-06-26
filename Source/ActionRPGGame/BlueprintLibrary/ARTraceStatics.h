// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "../Types/ARStructTypes.h"
#include "../Types/AREnumTypes.h"
#include "ARTraceStatics.generated.h"

UCLASS()
class UARTraceStatics : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintPure, Category = "AR|Trace")
		static FVector GetCameraAim(APawn* UsedBy);

	UFUNCTION(BlueprintPure, Category = "AR|Trace")
		static FVector GetCameraDamageStartLocation(const FVector& AimDir, APawn* UsedBy);

	UFUNCTION(BlueprintPure, Category = "AR|Trace")
		static USkeletalMeshComponent* GetWeaponMesh(APawn* InitiatedBy);

	UFUNCTION(BlueprintPure, Category = "AR|Trace")
		static FVector GetStartLocation(FName SocketName, APawn* InitiatedBy);

	UFUNCTION(BlueprintPure, Category = "AR|Trace")
		static FHitResult RangedTrace(const FVector& StartTrace, const FVector& EndTrace, APawn* InitiatedBy, TEnumAsByte<EARTraceType> TraceType);

	UFUNCTION(BlueprintCallable, Category = "AR|Trace")
		static FHitResult GetHitResult(float Range, FName StartSocket, APawn* InitiatedBy, bool DrawDebug, bool UseStartSocket, TEnumAsByte<EARTraceType> TraceType);
};



