// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"
#include "../Componenets/ARAttributeBaseComponent.h"
#include "AREffectPeriodic.h"

AAREffectPeriodic::AAREffectPeriodic(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	PeriodDuration = 1;
	CurrentDuration = 0;
	IsEffectActive = false;
}

void AAREffectPeriodic::TickMe(float DeltaTime)
{
	if (IsEffectActive)
	{
		CurrentDuration += DeltaTime;
		CurrentPeriodDuration += DeltaTime;
		if (CurrentPeriodDuration >= PeriodDuration)
		{
			CurrentPeriodDuration = 0;
			OnEffectPeriod();
		}
		if (CurrentDuration >= MaxDuration)
		{
			CurrentDuration = 0;
			IsEffectActive = false;
			Deactivate();
		}
	}
}
void AAREffectPeriodic::Initialze()
{
	if (Role < ROLE_Authority)
	{
		ServerActivate();
	}
	else
	{
		Activate();
	}
}

void AAREffectPeriodic::Activate()
{
	OnEffectInitialized();
	IsEffectActive = true;
}

void AAREffectPeriodic::ServerActivate_Implementation()
{
	Activate();
}

bool AAREffectPeriodic::ServerActivate_Validate()
{
	return true;
}

void AAREffectPeriodic::Deactivate()
{
	if (!EffectTarget)
		return;

	UARAttributeBaseComponent* AttrComp = EffectTarget->FindComponentByClass<UARAttributeBaseComponent>();

	if (!AttrComp)
		return;
	CurrentDuration = 0;
	IsEffectActive = false;
	OnEffectEnd();
	AttrComp->RemovePeriodicEffect(this);
}


