// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "AREffectPeriodic.generated.h"

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

		virtual void TickMe(float DeltaTime);

	bool IsEffectActive;

	float MaxDuration;
	UPROPERTY(Transient)
	float CurrentDuration;

	float PeriodDuration;
	UPROPERTY(Transient)
	float CurrentPeriodDuration;

	UPROPERTY(BlueprintReadOnly, Category = "Target")
		AActor* EffectCauser;

	UPROPERTY(BlueprintReadWrite, Category = "Target")
		AActor* EffectTarget;

	UFUNCTION()
		void Initialze();

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



