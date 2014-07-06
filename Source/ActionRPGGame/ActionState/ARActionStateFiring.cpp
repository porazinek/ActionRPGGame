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
		//if (IsFirstFire)
		//{
		//	IsFirstFire = false;
		//	GetOuterUARActionStateComponent()->CastBegin();
		//}
		//if (GetOuterUARActionStateComponent()->IsCasting == true)
		//	GetOuterUARActionStateComponent()->ServerSetCastingState(false);

		CurrentIntervalTime += DeltaTime;
		if (CurrentIntervalTime >= GetOuterUARActionStateComponent()->IntervalTime)
		{
			//GetOuterUARActionStateComponent()->ServerSetCastingState(true);
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
	GetOuterUARActionStateComponent()->GotoActiveState();
}