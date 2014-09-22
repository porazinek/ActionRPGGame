// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "ARActionState.h"

#include "ARActionStateCasting.generated.h"

/*
	For lulz I will implement basic state here,
	which is casting state.

	To test replication and client side prediction, as well as if it even works.
	Proper implementation one state - one class;
*/

UCLASS(DefaultToInstanced, Within = ARActionStateComponent)
class UARActionStateCasting : public UARActionState
{
	GENERATED_UCLASS_BODY()

		virtual void Tick(float DeltaTime) override;

	virtual void BeginState(UARActionState* PrevState) override;
	virtual void EndState() override;
	virtual void BeginActionSequence() override;
	virtual void EndActionSequence() override;

	FTimerHandle CastingHandle;

	void FireAction();

	bool IsCasting;

	FGameplayTagContainer TempTags;

};



