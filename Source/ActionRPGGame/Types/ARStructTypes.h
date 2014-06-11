// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "ARStructTypes.generated.h"


USTRUCT()
struct FEmptyStruct
{
	GENERATED_USTRUCT_BODY()
};

USTRUCT(BlueprintType)
struct FAttribute
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		FName AttributeName;

	UPROPERTY()
		float ModValue;

	FAttribute() {};
};

USTRUCT(BlueprintType)
struct FAttributeModified
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadOnly, Category="Attribute")
		FName AttributeName;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float ActuallDamage;

	FAttributeModified() {};
};

USTRUCT(BlueprintType)
struct FPeriodicEffect
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Effect")
		float CurrentDuration;

	UPROPERTY(NotReplicated, BlueprintReadOnly, Category = "Effect")
		float ClientCurrentDuration;

	UPROPERTY()
		float MaxDuration;

	/*
		This actually doesn't set damage to effect.
		It's used to replicate but to client to have something to show on UI. IF needed.
		Actuall damage is set directly in blueprint and is directly attached to used effects.
	*/
	UPROPERTY()
		float FinalDamage;

	UPROPERTY()
		FName EffectName;

	UPROPERTY()
		bool IsEffectActive;

	void Tick(float DeltaTime);

	/* We don't need to replicate effect object back to user
	   That is why struct contains most of data	*/
	UPROPERTY(NotReplicated, Transient)
		TWeakObjectPtr<class AAREffectPeriodic> PeriodicEffect;

	bool operator==(const FPeriodicEffect& Other) const;

	FPeriodicEffect()
		: CurrentDuration(0.0f),
		MaxDuration(0.0f),
		IsEffectActive(false),
		EffectName("Invalid")
	{ }
};

USTRUCT(BlueprintType)
struct FActivePeriodicEffects
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadOnly, Category = "Effect")
		TArray<FPeriodicEffect> ActiveEffects;
};