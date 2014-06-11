// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../Effects/AREffectPeriodic.h"
#include "../Componenets/ARAttributeComponent.h"
#include "AREffectStatics.h"

UAREffectStatics::UAREffectStatics(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}

FPeriodicEffect UAREffectStatics::CreatePeriodicEffect(AActor* EffectTarget, AActor* EffectCauser, float Duration, TSubclassOf<class AAREffectPeriodic> EffectType)
{
	FPeriodicEffect PeriodicEffect;
	if (!EffectTarget && !EffectCauser)
		return PeriodicEffect;

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoCollisionFail = true;
	SpawnInfo.Owner = EffectTarget;
	//SpawnInfo.Instigator = EffectCauser;

	AAREffectPeriodic* effecTemp = EffectTarget->GetWorld()->SpawnActor<AAREffectPeriodic>(EffectType, SpawnInfo);

	PeriodicEffect.PeriodicEffect = effecTemp;
	PeriodicEffect.MaxDuration = Duration;
	PeriodicEffect.PeriodicEffect->MaxDuration = Duration;
	return PeriodicEffect;
}

void UAREffectStatics::ActivatePeriodicEffect(FPeriodicEffect PeriodicEffect)
{
	//PeriodicEffect.PeriodicEffect->Initialze();
	UARAttributeComponent* attrComp = PeriodicEffect.PeriodicEffect->GetOwner()->FindComponentByClass<UARAttributeComponent>();
	PeriodicEffect.IsEffectActive = true;

	attrComp->AddPeriodicEffect(PeriodicEffect);
}

void UAREffectStatics::ChangeAttribute(AActor* Target, AActor* CausedBy, float ModVal, FName AttributeName, TEnumAsByte<EAttrOp> OpType)
{
	if (!Target && !CausedBy)
		return;

	TWeakObjectPtr<UARAttributeBaseComponent> attrComp = Target->FindComponentByClass<UARAttributeBaseComponent>();

	if (!attrComp.IsValid())
		return;

	attrComp->ChangeAttribute(ModVal, AttributeName, OpType);
}
