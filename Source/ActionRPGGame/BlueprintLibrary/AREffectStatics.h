// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "../Types/ARStructTypes.h"
#include "../Types/AREnumTypes.h"
#include "AREffectStatics.generated.h"

UCLASS()
class UAREffectStatics : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, Category = "AR|Static")
		static FPeriodicEffect CreatePeriodicEffect(AActor* EffectTarget, AActor* EffectCauser, float Duration, TSubclassOf<class AAREffectPeriodic> EffectType);
	UFUNCTION(BlueprintCallable, Category = "AR|Static")
		static void ActivatePeriodicEffect(FPeriodicEffect PeriodicEffect);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AR|Static")
		static void ChangeAttribute(AActor* Target, AActor* CausedBy, float ModVal, FName AttributeName, TEnumAsByte<EAttrOp> OpType);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AR|Static")
		static void ApplyPointAttributeChange(AActor* DamageTarget, float AttributeMod, FName AttributeName, TEnumAsByte<EAttrOp> AttributeOperation, FVector HitFromLocation, FHitResult HitInfo, AController* EventInstigator, AActor* Causer, TSubclassOf<class UDamageType> DamageType);
};



