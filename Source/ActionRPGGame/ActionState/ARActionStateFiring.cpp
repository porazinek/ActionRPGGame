// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "ARActionStateComponent.h"

#include "ARActionStateFiring.h"

UARActionStateFiring::UARActionStateFiring(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	FireStarted = false;
	IsFirstFire = true;
	CurrentIntervalTime = 0;
}

void UARActionStateFiring::Tick(float DeltaTime)
{
	if (FireStarted)
	{
		CurrentIntervalTime += DeltaTime;
		if (CurrentIntervalTime >= GetOuterUARActionStateComponent()->IntervalTime)
		{
			GetOuterUARActionStateComponent()->ActionInterval();
			//IsFirstFire = false;
			CurrentIntervalTime = 0;
		}
	}
}
void UARActionStateFiring::BeginState(UARActionState* PrevState)
{
	GetOuterUARActionStateComponent()->CastBegin();
	//GetOuterUARActionStateComponent()->IsCasting = false;
	FireStarted = true;
}
void UARActionStateFiring::EndState()
{
	FireStarted = false;
	GetOuterUARActionStateComponent()->CastEnd();
	CurrentIntervalTime = 0;
}

void UARActionStateFiring::BeginActionSequence()
{

}
void UARActionStateFiring::EndActionSequence()
{
	GetOuterUARActionStateComponent()->GotoState(GetOuterUARActionStateComponent()->RefireState);
}