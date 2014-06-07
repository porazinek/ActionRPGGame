// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "ARActionStateComponent.h"

#include "ARActionStateCasting.h"

UARActionStateCasting::UARActionStateCasting(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	GetOuterUARActionStateComponent();
}

void UARActionStateCasting::Tick(float DeltaTime)
{
	if (IsCasting)
	{
		CurrentCastTime += DeltaTime;
		if (CurrentCastTime >= GetOuterUARActionStateComponent()->MaxCastTime)
		{
			FireAction();
			//GetOuterUARActionStateComponent()->FireAction();
			GetOuterUARActionStateComponent()->GotoState(GetOuterUARActionStateComponent()->CooldownState);
		}
	}
}

void UARActionStateCasting::BeginState(UARActionState* PrevState)
{
	GetOuterUARActionStateComponent()->CastBegin();
	//GetOuterUARActionStateComponent()->GetOwner()->GetWorldTimerManager().SetTimer(this, &UARActionStateCasting::FireAction, GetOuterUARActionStateComponent()->MaxCastTime, false);
	IsCasting = true;
}
void UARActionStateCasting::FireAction()
{
	GetOuterUARActionStateComponent()->FireAction();
	//GetOuterUARActionStateComponent()->GotoState(GetOuterUARActionStateComponent()->ActiveState); //it should go to cooldown.
}
void UARActionStateCasting::EndState()
{
	GetOuterUARActionStateComponent()->GetOwner()->GetWorldTimerManager().ClearTimer(this, &UARActionStateCasting::FireAction);
	CurrentCastTime = 0;
	IsCasting = false;
}

void UARActionStateCasting::BeginActionSequence()
{

}
void UARActionStateCasting::EndActionSequence()
{

}