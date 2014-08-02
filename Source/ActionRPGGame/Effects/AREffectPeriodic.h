// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../Types/AREffectTypes.h"
#include "AREffectPeriodic.generated.h"
/*
	Dumb helpers, they will need some parameters.
	
	Helpers in case we need to access some of Effect, events outside of effect in blueprint.
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDMDOnEffectInitialized);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDMDOnEffectInterval);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDMDOnEffectRemoved);
/*
	Effect exist only on server. 
	Cosmetic side is handled trough AttributeComponent.

	I'm not sure if keeping it as AActor makes much sense, since it is not replicated nor have any 
	graphical effects visible, that would be tied directly to object in question.
*/

UCLASS(minimalapi)
class AAREffectPeriodic : public AActor
{
	GENERATED_UCLASS_BODY()
public:
	virtual void TickMe(float DeltaTime);
	virtual void PostNetReceive() override;
	UPROPERTY(ReplicatedUsing = OnRep_EffectActive)
	bool IsEffectActive;
	UFUNCTION()
		void OnRep_EffectActive();
	float MaxDuration;
	UPROPERTY(Transient)
	float CurrentDuration;

	float PeriodDuration;
	UPROPERTY(Transient)
	float CurrentPeriodDuration;

	UPROPERTY(BlueprintReadOnly, Category = "Target")
		AActor* EffectCauser;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Target")
		AActor* EffectTarget;
	UPROPERTY(EditAnywhere, Category = "Particles")
		TArray<FEffectCue> EffectCues;

	UPROPERTY(EditAnywhere, Category = "Effect Cues")
		TArray<TSubclassOf<class AARActorCue>> ActorCues;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* PresitentFX;
	UPROPERTY(EditAnywhere, Category = "Particles")
		FName AttachLocation;


	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* PeriodFX;

	UFUNCTION()
		void Initialze();
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Periodic Effect")
		FDMDOnEffectInitialized OnEffectActivated;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Periodic Effect")
		FDMDOnEffectInterval OnEffectInterval;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Periodic Effect")
		FDMDOnEffectRemoved OnEffectRemoved;

	UFUNCTION(BlueprintImplementableEvent, Category = "Effect")
		void OnEffectPeriod();
	UFUNCTION(BlueprintImplementableEvent, Category = "Effect")
		void OnEffectEnd();
	UFUNCTION(BlueprintImplementableEvent, Category = "Effect")
		void OnEffectInitialized();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerActivate();

	void Activate();

	void Deactivate();
};



