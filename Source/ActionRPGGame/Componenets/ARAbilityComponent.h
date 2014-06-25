// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../Types/ARStructTypes.h"

#include "ARAbilityComponent.generated.h"

UCLASS(hidecategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming), editinlinenew, meta = (BlueprintSpawnableComponent))
class UARAbilityComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UPROPERTY()
	class AARPlayerController* OwningController;

	UPROPERTY()
	class AARCharacter* OwningCharacter;

	UPROPERTY()
	class AARAbility* ActiveAbility;
	UPROPERTY()
	class AARAbility* ActionButtonOne;

	UPROPERTY(ReplicatedUsing=OnRep_ActionBarOne)
		TArray<FAbilityInfo> ActionBarOne;
	UFUNCTION()
		void OnRep_ActionBarOne();
	bool UpdateActionBarOne;


	//this would be probably best option.
	//but TMap is not replicared
	//and we need to display abilities on UI
	//in correct order for player convinience
	//even if it doesn't matter for input execution.
	TMap<FName, class AARAbility> EqAbilities;

	UFUNCTION(BlueprintCallable, Category = "Ability")
		void AddAbility(FAbilityInfo AbilityIn);

	//TEST!
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void AddAbilityToActionBar(FAbilityInfo AbilityIn, int32 SlotID);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAddAbilityToActionBar(FAbilityInfo AbilityIn, int32 SlotID);

};



