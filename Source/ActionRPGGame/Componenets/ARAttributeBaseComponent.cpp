// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../Types/ARStructTypes.h"
#include "../Types/AREnumTypes.h"
#include "../Effects/AREffectPeriodic.h"

#include "Net/UnrealNetwork.h"

#include "ARAttributeBaseComponent.h"

UARAttributeBaseComponent::UARAttributeBaseComponent(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = true;
}

void UARAttributeBaseComponent::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARAttributeBaseComponent, ActivePeriodicEffects);
	DOREPLIFETIME(UARAttributeBaseComponent, ModifiedAttribute);
}

void UARAttributeBaseComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*
	Client only.  When client get replicaated version of ActivePeriodicEffects, we run simulation on it.
	so we don't need to get timing from server each time.
	*/
	if (GetOwnerRole() < ROLE_Authority)
	{

		for (auto It = ActivePeriodicEffects.ActiveEffects.CreateIterator(); It; ++It)
		{
			//if (ActivePeriodicEffects.ActiveEffects[It.GetIndex()].PeriodicEffect == PeriodicEffect)
			//{
			//if (effect.IsEffectActive)
			//{
			ActivePeriodicEffects.ActiveEffects[It.GetIndex()].ClientCurrentDuration += DeltaTime;
			if (ActivePeriodicEffects.ActiveEffects[It.GetIndex()].ClientCurrentDuration >= ActivePeriodicEffects.ActiveEffects[It.GetIndex()].MaxDuration)
			{
				ActivePeriodicEffects.ActiveEffects[It.GetIndex()].ClientCurrentDuration = 0;
				ActivePeriodicEffects.ActiveEffects[It.GetIndex()].IsEffectActive = false;
			}
			//}
			//}
		}
		//for (FPeriodicEffect& effect : ActivePeriodicEffects.ActiveEffects)
		//{
		//	//if (effect.IsEffectActive)
		//	//{
		//		effect.ClientCurrentDuration += DeltaTime;
		//		if (effect.ClientCurrentDuration >= effect.MaxDuration)
		//		{
		//			effect.ClientCurrentDuration = 0;
		//			effect.IsEffectActive = false;
		//		}
		//	//}
		//}
	}
	//so we tick effects in struct. Maybe it's not best idea, but it's the only one I have right now...
	//we need to tick them on client for cosmetic as well
	if (GetOwnerRole() == ROLE_Authority)
	{
		for (FPeriodicEffect& effect : ActivePeriodicEffects.ActiveEffects)
		{
			effect.Tick(DeltaTime);
		}
	}
}

void UARAttributeBaseComponent::AddPeriodicEffect(FPeriodicEffect PeriodicEffect)
{
	/*
	there are two solutions. Create client only effect
	and just create server only and replicate it back to client.
	*/
	if (GetOwnerRole() < ROLE_Authority)
	{
		//server function here should be - Yoda
		//PeriodicEffect.PeriodicEffect->Initialze();
		//PeriodicEffect.IsEffectActive = true;
		//ActiveClientEffects.AddUnique(PeriodicEffect);
	}
	else
	{
		//server
		PeriodicEffect.PeriodicEffect->Initialze();
		ActivePeriodicEffects.ActiveEffects.AddUnique(PeriodicEffect);
	}

}

void UARAttributeBaseComponent::RemovePeriodicEffect(class AAREffectPeriodic* PeriodicEffect)
{
	for (auto It = ActivePeriodicEffects.ActiveEffects.CreateIterator(); It; ++It)
	{
		if (ActivePeriodicEffects.ActiveEffects[It.GetIndex()].PeriodicEffect.Get() == PeriodicEffect)
		{
			//ActivePeriodicEffects.ActiveEffects[It.GetIndex()].PeriodicEffect->Destroy();
			ActivePeriodicEffects.ActiveEffects.RemoveAtSwap(It.GetIndex());
			//PeriodicEffect->Deactivate();
			//PeriodicEffect->
			return;
		}
	}
}

UProperty* UARAttributeBaseComponent::GetAttribute(FName AttributeName)
{
	//we could perform caching for pointer, maybe later.
	return FindFieldChecked<UProperty>(this->GetClass(), AttributeName);
}

float UARAttributeBaseComponent::GetFloatValue(FName AttributeName)
{
	if (AttributeName.IsNone())
		return 0;

	UNumericProperty* NumericProperty = CastChecked<UNumericProperty>(GetAttribute(AttributeName));
	void* pValue = NumericProperty->ContainerPtrToValuePtr<void>(this);
	return NumericProperty->GetFloatingPointPropertyValue(pValue);
}

void UARAttributeBaseComponent::SetFloatValue(float InValue, FName AttributeName)
{
	if (AttributeName.IsNone())
		return;

	UNumericProperty* NumericProperty = CastChecked<UNumericProperty>(GetAttribute(AttributeName));
	void* pValue = NumericProperty->ContainerPtrToValuePtr<void>(this);
	NumericProperty->SetFloatingPointPropertyValue(pValue, InValue);
}

int32 UARAttributeBaseComponent::GetIntValue(FName AttributeName)
{
	if (AttributeName.IsNone())
		return 0;
	UIntProperty* intProperty = CastChecked<UIntProperty>(GetAttribute(AttributeName));

	//UNumericProperty* NumericProperty = CastChecked<UNumericProperty>(GetAttribute(AttributeName));
	void* pValue = intProperty->ContainerPtrToValuePtr<void>(this);
	return intProperty->GetUnsignedIntPropertyValue(pValue);
}

void UARAttributeBaseComponent::SetIntValue(int32 InValue, FName AttributeName)
{

}

void UARAttributeBaseComponent::ChangeAttribute(float ModValue, FName AttributeName, TEnumAsByte<EAttrOp> OpType)
{
	float attrValue = GetFloatValue(AttributeName);
	ModifiedAttribute.ActuallDamage = ModValue;
	ModifiedAttribute.AttributeName = AttributeName;
	SetAttributeModified(ModValue, AttributeName);
	SetFloatValue(AttributeOp(ModValue, attrValue, OpType), AttributeName);
}
void UARAttributeBaseComponent::SetAttributeModified(float ModValue, FName AttributeName)
{
	FAttributeModified ModedAttribute;
	ModedAttribute.ActuallDamage = ModValue;
	ModedAttribute.AttributeName = AttributeName;
	OnAttributeModified.Broadcast(ModedAttribute);
}

void UARAttributeBaseComponent::SetAttributeModifiedTwo()
{
	OnAttributeModified.Broadcast(ModifiedAttribute);
}

float UARAttributeBaseComponent::AttributeOp(float ModValue, float AttrValueIn, TEnumAsByte<EAttrOp> OpType)
{
	switch (OpType)
	{
	case EAttrOp::Attr_Add:
		return AttrValueIn += ModValue;
	case EAttrOp::Attr_Subtract:
		return AttrValueIn -= ModValue;
	case EAttrOp::Attr_Multiply:
		return AttrValueIn *= ModValue;
	case EAttrOp::Attr_Divide:
		return AttrValueIn = (AttrValueIn / ModValue);
	case EAttrOp::Attr_Set:
		return AttrValueIn = ModValue;
	default:
		return 0;
	}
}

