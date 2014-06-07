// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "ARActionStateComponent.h"

#include "ARActionState.h"

UARActionState::UARActionState(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	GetOuterUARActionStateComponent();
}

void UARActionState::Tick(float DeltaTime)
{

}
void UARActionState::BeginState(UARActionState* PrevState)
{

}
void UARActionState::EndState()
{
	CurrentCastTime = 0;
}

void UARActionState::BeginActionSequence()
{

}
void UARActionState::EndActionSequence()
{

}