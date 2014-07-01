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
	UARAttributeComponent* attrComp = EffectTarget->FindComponentByClass<UARAttributeComponent>();

	if (attrComp->ActivePeriodicEffects.ActiveEffects.Num() > 0)
	{
		for (FPeriodicEffect& effect : attrComp->ActivePeriodicEffects.ActiveEffects)
		{
			if (effect.PeriodicEffect.IsValid() && effect.PeriodicEffect->GetClass() == EffectType)
			{
				//reality. Do check if effect is stackable or something.
				return PeriodicEffect;
			}
		}
	}

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoCollisionFail = true;
	SpawnInfo.Owner = EffectTarget;
	//SpawnInfo.Instigator = EffectCauser;

	AAREffectPeriodic* effecTemp = EffectTarget->GetWorld()->SpawnActor<AAREffectPeriodic>(EffectType, SpawnInfo);
	effecTemp->EffectCauser = EffectCauser;
	PeriodicEffect.PeriodicEffect = effecTemp;
	PeriodicEffect.MaxDuration = Duration;
	PeriodicEffect.PeriodicEffect->MaxDuration = Duration;

	
	PeriodicEffect.IsEffectActive = true;

	attrComp->AddPeriodicEffect(PeriodicEffect);

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
	attrComp->ChangeAttribute(AttributeName, ModVal, OpType);
}

void UAREffectStatics::ApplyDamage(AActor* DamageTarget, float BaseDamage, FName AttributeName, AActor* EventInstigator, AActor* DamageCauser, TSubclassOf<class UDamageType> DamageType)
{
	if (!DamageTarget)
		return;

	TWeakObjectPtr<UARAttributeBaseComponent> attr = DamageTarget->FindComponentByClass<UARAttributeBaseComponent>();

	FAttribute Attribute;
	Attribute.AttributeName = AttributeName;
	Attribute.ModValue = BaseDamage;
	Attribute.OperationType = EAttrOp::Attr_Subtract;

	FARDamageEvent DamageEvent;
	DamageEvent.Attribute = Attribute;
	DamageEvent.DamageTypeClass = DamageType;

	attr->DamageAttribute(DamageEvent, EventInstigator, DamageCauser);
}

void UAREffectStatics::ApplyPointDamage(AActor* DamageTarget, float AttributeMod, FName AttributeName, const FVector& HitFromLocation, const FHitResult& HitInfo, AActor* EventInstigator, AActor* Causer, TSubclassOf<class UDamageType> DamageType)
{
	if (!DamageTarget)
		return;

	TWeakObjectPtr<UARAttributeBaseComponent> attrComp = DamageTarget->FindComponentByClass<UARAttributeBaseComponent>();

	if (!attrComp.IsValid())
		return;

	FAttribute Attribute;
	Attribute.AttributeName = AttributeName;
	Attribute.ModValue = AttributeMod;
	Attribute.OperationType = EAttrOp::Attr_Subtract;

	//FPointAttributeChangeEvent AttributeEvent(Attribute, HitInfo, HitFromLocation, DamageType);
	FARPointDamageEvent AttributeEvent;
	AttributeEvent.Attribute = Attribute;
	AttributeEvent.HitInfo = HitInfo;
	AttributeEvent.ShotDirection = HitFromLocation;
	AttributeEvent.DamageTypeClass = DamageType;
	attrComp->DamageAttribute(AttributeEvent, EventInstigator, Causer);
}

void UAREffectStatics::ApplyRadialDamageWithFalloff(UObject* WorldContextObject, float BaseDamage, float MinimumDamage, const FVector& Origin, float DamageInnerRadius, float DamageOuterRadius, float DamageFalloff, TSubclassOf<class UDamageType> DamageTypeClass, const TArray<AActor*>& IgnoreActors, AActor* DamageCauser, AActor* Instigator)
{

}

void UAREffectStatics::ApplyRadialDamage(UObject* WorldContextObject, float BaseDamage, const FVector& Origin, float DamageRadius, TSubclassOf<class UDamageType> DamageTypeClass, const TArray<AActor*>& IgnoreActors, AActor* DamageCauser, AActor* Instigator, bool bDoFullDamage)
{

}