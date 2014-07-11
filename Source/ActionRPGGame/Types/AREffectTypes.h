// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "AREnumTypes.h"
#include "Slate.h"
#include "AREffectTypes.generated.h"

USTRUCT(BlueprintType)
struct FEffectHandle
{
	GENERATED_USTRUCT_BODY()
public:
	inline bool operator==(const FEffectHandle& Other) const
	{
		return Handle == Other.Handle;
	};
	inline bool operator!=(const FEffectHandle& Other) const
	{
		return Handle != Other.Handle;
	};

	FEffectHandle() {};
private:
	int32 Handle;
};

/*
Stores information about cosmetic effects that might be appiled by weapons, abilities or effects.
*/
USTRUCT(BlueprintType)
struct FEffectCue
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Effect Cue")
		FName CueName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Cue")
		TWeakObjectPtr<UParticleSystem> ParticleSystem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Cue")
		TWeakObjectPtr<USoundBase> Sound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Cue")
		TWeakObjectPtr<UAnimMontage> AnimMontage;
};

//this struct will need clean up. I toss random things here for testing!
USTRUCT(BlueprintType)
struct FEffectSpec
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	FEffectHandle Handle;

	//will match name in FEffectData.
	UPROPERTY()
		FName EffectName;

	//UPROPERTY()
	//class UAREffectPeriodicO* Effect;

	UPROPERTY(BlueprintReadWrite, Category = "Effect")
		FEffectCue EffectCue;

	UPROPERTY()
		FGameplayTagContainer OwnedTags;

	UPROPERTY() //really don't think that this is needed.
		TSubclassOf<class AARActorCue> ActorCue;
	UPROPERTY()
	class AAREffectPeriodic* ActorEffect;
	UPROPERTY()
		bool IsActive;

	UPROPERTY(BlueprintReadOnly, Category = "Effect")
		float CurrentDuration;
	UPROPERTY(BlueprintReadOnly, Category = "Effect")
		float MaxDuration;
};

USTRUCT(BlueprintType)
struct FActiveEffect
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	FEffectHandle Handle;

	UPROPERTY(NotReplicated)
	class UAREffectPeriodicO* Effect;

	UPROPERTY()
		bool IsActive;

	UPROPERTY(BlueprintReadOnly, Category = "Effect")
		float CurrentDuration;
	UPROPERTY(BlueprintReadOnly, Category = "Effect")
		float MaxDuration;
};

USTRUCT(BlueprintType)
struct FActiveEffectList
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "Effect")
		TArray<FEffectSpec> Effects;
};

/*
	We will use for async loading of cosmetic effcts on clients, while effect is appiled.
*/
USTRUCT(BlueprintType)
struct FEffectData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
		UParticleSystem* EffectParticle;

	UPROPERTY()
		UAnimMontage* EffectAnim;


};