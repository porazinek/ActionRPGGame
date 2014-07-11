// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "ARActorCue.generated.h"

/*
	Will use it to attach multiple comsetic effects to single target.

	Other possible solution would be to create TArray of cosmetic effect on target actor,
	and just add effects to it.
	
	After effect ends, this actor is going to be immiedetly destroyed.
	In theory at least ;).
*/
UCLASS(minimalapi)
class AARActorCue : public AActor
{
	GENERATED_UCLASS_BODY()
public:
	TSubobjectPtr<UArrowComponent> ArrowRoot;

	UFUNCTION()
		void InitializeAttachment(AActor* AttachToIn, FName AttachNameIn);
	UPROPERTY()
	TWeakObjectPtr<AActor> AttachTo;
	UPROPERTY()
	FName AttachName;

	UPROPERTY()
	TWeakObjectPtr<class UParticleSystem> ParticleSystem;
	UPROPERTY()
	TWeakObjectPtr<class UParticleSystemComponent> ParticleSystemComp;
};



