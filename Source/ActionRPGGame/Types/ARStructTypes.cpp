// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "ActionRPGGame.h"

#include "../Effects/AREffectPeriodic.h"

#include "ARStructTypes.h"

void FPeriodicEffect::Tick(float DeltaTime)
{
	//this is for server;
	if (PeriodicEffect.IsValid())
	{
		PeriodicEffect->TickMe(DeltaTime);
	}

	//this is for gui cosmetics;
	if (IsEffectActive)
	{
		CurrentDuration += DeltaTime;
		if (CurrentDuration >= MaxDuration)
		{
			CurrentDuration = 0;
			IsEffectActive = false;
		}
	}
}

bool FPeriodicEffect::operator==(const FPeriodicEffect& Other) const
{
	return PeriodicEffect->GetClass() == Other.PeriodicEffect->GetClass();
}