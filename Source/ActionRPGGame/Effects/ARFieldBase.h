// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "ARFieldBase.generated.h"

USTRUCT(BlueprintType)
struct FFieldTags
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "Tags")
		FGameplayTagContainer OwnedTags;
	UPROPERTY(BlueprintReadOnly, Category = "Tags")
		FGameplayTagContainer RequiredTags;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FDMDOnBeginFieldOverlap, FFieldTags, OtherTags, class AActor*, OtherActor, class UPrimitiveComponent*, OtherComp, bool, bFromSweep, const FHitResult &, SweepResult);

/*
	Base class for Field.
	Field is special actor which can be spawned by other actor and which presist in game world:
	1. Until it's life time expire.
	2. Until it takes to much damage.
	3. Until it is triggered. (like proximity mine).
	4. Other conditions might be defined as needed.

	Fields can interact with other fields. For example fire field can be extinguished by wind (or spread by it)
	or by water.
	Oil field can be burned by fire, etc.

	Fields are visible by everyone, if they have any visible componenets (Particle effects, meshes etc).

	There probably needs to be another object type, which will react to various types of damage as well ;).
*/

UCLASS()
class AARFieldBase : public AActor
{
	GENERATED_UCLASS_BODY()
public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void Initialize();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Componenets")
	TSubobjectPtr<class USphereComponent> Collision;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componenets")
	//	TSubobjectPtr<class UStaticMeshComponent> Mesh;
		//How much time must pass between ticks.
		//Usefull for applying damage or area search (SweepMulti) to check
		//if any actors are within field radius.
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Properties")
		float TickInterval;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Properties")
		bool IsTicking;


	/*
		Maxium life time in seconds.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Properties")
		float MaximumLifeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Properties")
		bool IsLifeInfinite;

	/*
		My Tags
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
		FGameplayTagContainer OwnedTags;
	/*
		Tags I require on other field to make any interaction with it.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
		FGameplayTagContainer RequiredTags;

	//UPROPERTY(BlueprintReadOnly, Category = "Field")
	//	TArray<class AARCharacter*> CharactersInField;

	/*
		Event Delgate that contain information about what overlaped our field.
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Field")
		FDMDOnBeginFieldOverlap OnBeginFieldOverlap;

	UFUNCTION(BlueprintImplementableEvent, Category = "Field")
		void OnFieldInitialized();

	UFUNCTION(BlueprintImplementableEvent, Category = "Field")
		void OnFieldBeginDestroy();

	UFUNCTION(BlueprintImplementableEvent, Category = "Field")
		void OnFieldTick();

	/*
		Spawn combo field in place of current one. 
		Use to create combos of two overlapping fields.
	*/
	UFUNCTION(BlueprintCallable, Category = "AR|Field")
		void SpawnField(TSubclassOf<class AARFieldBase> Field, float Duration, float TickIntervalIn, FVector Location, AARFieldBase* OtherField);
private:

	float CurrentLifeTime;
	FTimerHandle LifeHandle;
	FTimerHandle IntervalHandle;

	/*
		Helper to set values in OnBeginFieldOverlap delegate
	*/
	UFUNCTION()
		void SetOnBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SelfClean();

	void SetFieldTick();
};



