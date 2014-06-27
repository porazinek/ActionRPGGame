// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "ARActionStateComponent.h"

#include "ARActionStateFiring.h"

UARActionStateFiring::UARActionStateFiring(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	FireStarted = false;
	IsFirstFire = true;
}

void UARActionStateFiring::Tick(float DeltaTime)
{
	if (FireStarted)
	{
		if (IsFirstFire)
			GetOuterUARActionStateComponent()->CastBegin();

		CurrentIntervalTime += DeltaTime;
		if (CurrentIntervalTime >= GetOuterUARActionStateComponent()->IntervalTime)
		{
			GetOuterUARActionStateComponent()->ActionInterval();
			IsFirstFire = false;
			CurrentCastTime = 0;
		}
	}
}
void UARActionStateFiring::BeginState(UARActionState* PrevState)
{
	FireStarted = true;
}
void UARActionStateFiring::EndState()
{
	FireStarted = false;
}

void UARActionStateFiring::BeginActionSequence()
{

}
void UARActionStateFiring::EndActionSequence()
{
	GetOuterUARActionStateComponent()->GotoActiveState();
}