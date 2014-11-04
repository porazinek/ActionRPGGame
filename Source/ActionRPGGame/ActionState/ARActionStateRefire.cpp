// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "ARActionStateComponent.h"

#include "ARActionStateRefire.h"

UARActionStateRefire::UARActionStateRefire(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CurrentCooldownTime = 0;
	IsOnCooldown = false;
}

void UARActionStateRefire::Tick(float DeltaTime)
{
	if (IsOnCooldown)
	{
		CurrentCooldownTime += DeltaTime;
		float ActionCooldownTime = GetOuterUARActionStateComponent()->RefireTime;
		if (CurrentCooldownTime >= ActionCooldownTime)
		{
			CooldownEnded();
			GetOuterUARActionStateComponent()->GotoState(GetOuterUARActionStateComponent()->ActiveState);
		}
	}
}
void UARActionStateRefire::BeginState(UARActionState* PrevState)
{
	GetOuterUARActionStateComponent()->CooldownBegin();
	IsOnCooldown = true;
}
void UARActionStateRefire::EndState()
{
	IsOnCooldown = false;
	CurrentCooldownTime = 0;
}

void UARActionStateRefire::CooldownEnded()
{
	GetOuterUARActionStateComponent()->CooldownEnded();
}

void UARActionStateRefire::BeginActionSequence()
{

}
void UARActionStateRefire::EndActionSequence()
{

}
