// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "../Types/ARStructTypes.h"
#include "../Types/AREnumTypes.h"
#include "AREffectStatics.generated.h"

UCLASS()
class UAREffectStatics : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AR|Static")
		static FEffectSpec CreatePeriodicEffect(AActor* EffectTarget, AActor* EffectCauser, float Duration, TSubclassOf<class AAREffectPeriodic> EffectType);
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AR|Static")
		static void ActivatePeriodicEffect(FPeriodicEffect PeriodicEffect);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AR|Static")
		static void ApplyPeriodicEffect(AActor* EffectTarget, AActor* EffectCauser, float Duration, TSubclassOf<class UAREffectPeriodicO> EffectType);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AR|Damage Static")
		static void ChangeAttribute(AActor* Target, AActor* CausedBy, float ModVal, FName AttributeName, TEnumAsByte<EAttrOp> OpType);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AR|Damage Static")
		static void ApplyDamage(AActor* DamageTarget, float BaseDamage, FName AttributeName, AActor* EventInstigator, AActor* DamageCauser, FGameplayTagContainer DamageTag, TSubclassOf<class UDamageType> DamageType);
	/*
		EventInstigator - who Instigated Damage. In General should be controller. But can be any Actor with Attribute component.
	*/
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AR|Damage Static")
		static void ApplyPointDamage(AActor* DamageTarget, float AttributeMod, FName AttributeName, const FVector& HitFromLocation, const FHitResult& HitInfo, AActor* EventInstigator, AActor* Causer, TSubclassOf<class UDamageType> DamageType);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AR|Damage Static", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", AutoCreateRefTerm = "IgnoreActors"))
		static void ApplyRadialDamage(UObject* WorldContextObject, float BaseDamage, const FVector& Origin, float DamageRadius, TSubclassOf<class UDamageType> DamageTypeClass, const TArray<AActor*>& IgnoreActors, AActor* DamageCauser, AActor* Instigator, bool bDoFullDamage);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AR|Damage Static", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", AutoCreateRefTerm = "IgnoreActors"))
		static void ApplyRadialDamageWithFalloff(UObject* WorldContextObject, float BaseDamage, float MinimumDamage, const FVector& Origin, float DamageInnerRadius, float DamageOuterRadius, float DamageFalloff, TSubclassOf<class UDamageType> DamageTypeClass, const TArray<AActor*>& IgnoreActors, AActor* DamageCauser = NULL, AActor* Instigator = NULL);


	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AR|Damage Static")
		static void ShootProjectile(TSubclassOf<class AARProjectile> Projectile, FVector Origin, FVector ShootDir, AActor* Causer, FName StartSocket, FHitResult HitResult);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AR|Damage Static")
		static void SpawnProjectileInArea(TSubclassOf<class AARProjectile> Projectile, AActor* Causer, APawn* Instigator, FHitResult HitResult, float InitialVelocity, float MaxRadius, float MaxHeight, float ImpactDirection, int32 Amount);
};



