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
	//Pass ability info here ?
	//how Do I distinguish type of damage.
	//Or is it healing ?
	//In UI might want to color code damage numbers depending on damage type.
	//Or display ability/effect icons along damage numbers.
	//Do I want floating damage numbers in the first place ?

	//I will need data struct, that will be constructed here
	//then passed to component
	//and the replicated and broadcasted using delegate. (also broadcasted for server as well).


	attrComp->ChangeAttribute(ModVal, AttributeName, OpType);
}
