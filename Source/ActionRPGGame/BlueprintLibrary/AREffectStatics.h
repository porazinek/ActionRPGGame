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
		static FEffectSpec CreatePeriodicEffect(AActor* EffectTarget, AActor* EffectCauser, float Duration, TSubclassOf<class AAREffectPeriodic> EffectType, FEffectCue EffectCue, TSubclassOf<class AARActorCue> ActorCue);
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AR|Static")
		static void ActivatePeriodicEffect(FPeriodicEffect PeriodicEffect);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AR|Static")
		static void ApplyPeriodicEffect(AActor* EffectTarget, AActor* EffectCauser, float Duration, TSubclassOf<class UAREffectPeriodicO> EffectType);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AR|Damage Static")
		static void ChangeAttribute(AActor* Target, AActor* CausedBy, float ModVal, FName AttributeName, TEnumAsByte<EAttrOp> OpType);

	/*
		Ahem. I want to add seprate Heal Damage functionality. which means:
		1. I have to copy paste everything. Bad.
		2. Extract the target aggregation part to seprate functions, and just create simple function for healing
		and damage.
		3. Add switch to indicate whether it is healing or dmage to existing functions.

		Also probably will need to add few more Damage Trace Types:
		1. Positioned Box - Box is spawned in position aligned to player view in hit point.
		2. Cylinder ?
	*/
	/*
	
	*/
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AR|Damage Static")
		static void ApplyDamage(AActor* DamageTarget, float BaseDamage, FName AttributeName, AActor* EventInstigator, AActor* DamageCauser, FGameplayTagContainer DamageTag, TSubclassOf<class UDamageType> DamageType);
	/*
		EventInstigator - who Instigated Damage. In General should be controller. But can be any Actor with Attribute component.
	*/
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AR|Damage Static")
		static void ApplyPointDamage(AActor* DamageTarget, float AttributeMod, FName AttributeName, const FVector& HitFromLocation, const FHitResult& HitInfo, AActor* EventInstigator, AActor* Causer, TSubclassOf<class UDamageType> DamageType);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AR|Damage Static", meta = (AutoCreateRefTerm = "IgnoreActors"))
		static void ApplyRadialDamage(FName AttributeName, float BaseDamage, const FVector& Origin, TEnumAsByte<ECollisionChannel> Collision, float DamageRadius, TSubclassOf<class UDamageType> DamageTypeClass, const TArray<AActor*>& IgnoreActors, AActor* DamageCauser, AActor* Instigator, bool bDoFullDamage, FGameplayTagContainer DamageTag);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AR|Damage Static", meta = (AutoCreateRefTerm = "IgnoreActors"))
		static void ApplyRadialDamageWithFalloff(FName AttributeName, float BaseDamage, float MinimumDamage, const FVector& Origin, TEnumAsByte<ECollisionChannel> Collision, float DamageInnerRadius, float DamageOuterRadius, float DamageFalloff, TSubclassOf<class UDamageType> DamageTypeClass, const TArray<AActor*>& IgnoreActors, FGameplayTagContainer DamageTag, AActor* DamageCauser = NULL, AActor* Instigator = NULL);
	
	/*
		Apply Damage in box shape from Origin point to end of trace.
	*/
	UFUNCTION(BlueprintCallable, Category = "AR|Damage Static")
		static void ApplyMultiBoxDamage(FVector StartLocation, float Range, FVector BoxExtends, TEnumAsByte<ECollisionChannel> Collision, AActor* DamageCauser, APawn* DamageInstigator);
	//probably need better category
	/*
	
	*/
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AR|Spawn Helpers")
		static void ShootProjectile(TSubclassOf<class AARProjectile> Projectile, FVector Origin, FVector ShootDir, AActor* Causer, FName StartSocket, FHitResult HitResult);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AR|Spawn Helpers")
		static void SpawnProjectileInArea(TSubclassOf<class AARProjectile> Projectile, AActor* Causer, APawn* Instigator, FHitResult HitResult, float InitialVelocity, float MaxRadius, float MaxHeight, float ImpactDirection, int32 Amount);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AR|Spawn Helpers")
		static void SpawnField(TSubclassOf<class AARFieldBase> Field, AActor* Instigator, FHitResult Location, float Duration, float TickInterval);
};



