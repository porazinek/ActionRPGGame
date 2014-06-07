// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "ARActionState.generated.h"

/*
	For lulz I will implement basic state here,
	which is casting state.

	To test replication and client side prediction, as well as if it even works.
	Proper implementation one state - one class;
*/

UCLASS(DefaultToInstanced, Within = ARActionStateComponent)
class UARActionState : public UObject
{
	GENERATED_UCLASS_BODY()

	virtual void Tick(float DeltaTime);

	virtual void BeginState(UARActionState* PrevState);
	virtual void EndState();
	virtual void BeginActionSequence();
	virtual void EndActionSequence();


	float CurrentCastTime;

};



