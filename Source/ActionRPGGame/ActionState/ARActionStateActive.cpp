// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "ARActionStateComponent.h"

#include "ARActionStateActive.h"

UARActionStateActive::UARActionStateActive(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
}

void UARActionStateActive::Tick(float DeltaTime)
{

}
void UARActionStateActive::BeginState(UARActionState* PrevState)
{

}
void UARActionStateActive::EndState()
{
}

void UARActionStateActive::BeginActionSequence()
{
	if (GetOuterUARActionStateComponent())
	{
		GetOuterUARActionStateComponent()->GotoState(GetOuterUARActionStateComponent()->StartActionState);
		return;
	}
}
void UARActionStateActive::EndActionSequence()
{

}