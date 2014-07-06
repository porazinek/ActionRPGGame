// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "ARActionStateCooldown.h"

UARActionStateCooldown::UARActionStateCooldown(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	CurrentCooldownTime = 0;
	IsOnCooldown = false;
}

void UARActionStateCooldown::Tick(float DeltaTime)
{
	if (IsOnCooldown)
	{
		CurrentCooldownTime += DeltaTime;
		float ActionCooldownTime = GetOuterUARActionStateComponent()->ActionCooldownTime;
		if (CurrentCooldownTime >= ActionCooldownTime)
		{
			CooldownEnded();
			GetOuterUARActionStateComponent()->GotoState(GetOuterUARActionStateComponent()->ActiveState);
		}
	}
}
void UARActionStateCooldown::BeginState(UARActionState* PrevState)
{
	GetOuterUARActionStateComponent()->CooldownBegin();
	IsOnCooldown = true;
}
void UARActionStateCooldown::EndState()
{
	IsOnCooldown = false;
	CurrentCooldownTime = 0;
}

void UARActionStateCooldown::CooldownEnded()
{
	GetOuterUARActionStateComponent()->CooldownEnded();
}

void UARActionStateCooldown::BeginActionSequence()
{

}
void UARActionStateCooldown::EndActionSequence()
{

}
