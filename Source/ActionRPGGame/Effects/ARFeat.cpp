// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"
#include "ARFeatComponent.h"
#include "ARFeat.h"

#include "../Items/Weapons/ARRangedWeapon.h"

UARFeat::UARFeat(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}
void UARFeat::InitializeEffect()
{
	Feats = GetOuterUARFeatComponent();
	GetOuterUARFeatComponent()->OnOutgoingDamage.AddDynamic(this, &UARFeat::ModifyOutgoingDamage);
}

void UARFeat::ModifyOutgoingDamage_Implementation(float DamageIn, AActor* ModdedActor)
{
	//return 9;
	AARRangedWeapon* weapon = Cast<AARRangedWeapon>(ModdedActor);
	if (weapon)
	{
		weapon->Damage += 10;
	}
}

void UARFeat::ModifyIncomingActor_Implementation(AActor* ActorIn)
{

}