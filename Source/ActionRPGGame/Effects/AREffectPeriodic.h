// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "AREffectPeriodic.generated.h"
/*
	Dumb helpers, they will need some parameters.
	
	Helpers in case we need to access some of Effect, events outside of effect in blueprint.
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDMDOnEffectInitialized)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDMDOnEffectInterval)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDMDOnEffectRemoved)
/*
	Logic of effect is executed only on server. Although Periodic effect is spawned on both
	cliet and server, client only receive cosmetic non-authorative stuff, that is
	as for not not really synced with what is going on on server.
	This needs to be fixed.

	Server should have ability to override client effect. 
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
		UParticleSystem* PresitentFX;
	UPROPERTY(EditAnywhere, Category = "Particles")
		FName AttachLocation;


	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* PeriodFX;

	UFUNCTION()
		void Initialze();
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Periodic Effect")
		FDMDOnEffectInitialized OnEffectInitialized;
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



