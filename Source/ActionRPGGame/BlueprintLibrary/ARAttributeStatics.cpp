// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../Effects/AREffectPeriodic.h"
#include "../Componenets/ARAttributeComponent.h"
#include "ARTraceStatics.h"
#include "ARAttributeStatics.h"

UARAttributeStatics::UARAttributeStatics(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}

float UARAttributeStatics::ReduceDamageByArmor(AActor* DamageTarget, AActor* Instigator, float DamageIn)
{
	if (!DamageTarget && !Instigator)
		return 0;

	TWeakObjectPtr<UARAttributeBaseComponent> tarAttr = DamageTarget->FindComponentByClass<UARAttributeBaseComponent>();
	TWeakObjectPtr<UARAttributeBaseComponent> instAttr = Instigator->FindComponentByClass<UARAttributeBaseComponent>();

	float Armor = tarAttr->GetFloatValue("Armor");

	return DamageIn -= Armor;
}