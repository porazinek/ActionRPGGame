// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../Types/ARStructTypes.h"

#include "ARAbilityComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnActionAddedToBar, FActionSlotInfo);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDMDOnAbilityAdded, class AARAbility*, AbilityIn);

UCLASS(hidecategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming), editinlinenew, meta = (BlueprintSpawnableComponent))
class UARAbilityComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()
public:
	virtual void InitializeComponent() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = "Ability Data Temp")
		TArray<FARAbilityData> AbiltityDataTemp;

	UPROPERTY()
	class AARPlayerController* OwningController;

	UPROPERTY()
	class AARCharacter* OwningCharacter;

	UPROPERTY()
	class AARAbility* ActiveAbility;
	UPROPERTY()
	class AARAbility* ActionButtonOne;

	UPROPERTY(EditAnywhere, Category = "Ability List Temp")
		TArray<FActionSlotInfo> AbilityBook;
	
	UPROPERTY()
		FActionSlotInfo ActiveAction;

	UPROPERTY(EditAnywhere, Replicated, Category = "Action Bar")
		FActionBarContainer ActionBars;

	UPROPERTY(ReplicatedUsing=OnRep_ActionBarOne)
		TArray<FAbilityInfo> ActionBarOne;
	UFUNCTION()
		void OnRep_ActionBarOne();

	void SetActiveAction(FActionSlotInfo ActionIn);

	bool UpdateActionBarOne;


	FOnActionAddedToBar OnActionAddedToBar;

	UPROPERTY(BlueprintCallable, Category = "Ability")
		FDMDOnAbilityAdded OnAbilityAdded;

	//this would be probably best option.
	//but TMap is not replicared
	//and we need to display abilities on UI
	//in correct order for player convinience
	//even if it doesn't matter for input execution.
	TMap<FName, class AARAbility> EqAbilities;

	UFUNCTION(BlueprintCallable, Category = "Ability")
		void AddAbility(FAbilityInfo AbilityIn);

	UFUNCTION()
		void SetActiveAbility(int32 SlotID, int32 ActionBarIndex);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetActiveAbility(int32 SlotID, int32 ActionBarIndex);
	//TEST!
	UFUNCTION(BlueprintCallable, Category = "Ability")
		void AddAbilityToActionBar(FActionSlotInfo AbilityIn, int32 SlotID, int32 ActionBarIndex);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAddAbilityToActionBar(FActionSlotInfo AbilityIn, int32 SlotID, int32 ActionBarIndex);

	UFUNCTION()
		void FireAbility();
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerFireAbility();

	UFUNCTION(Client, Reliable)
		void ClientOnAbilityAdded(int32 SlotID, int32 ActionBarIndex);

};



