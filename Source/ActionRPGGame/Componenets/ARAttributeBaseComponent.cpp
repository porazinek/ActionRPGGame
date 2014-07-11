// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../Types/ARStructTypes.h"
#include "../Types/AREnumTypes.h"
#include "../Effects/AREffectPeriodic.h"
#include "../ARCharacter.h"
#include "../ARPlayerCameraManager.h"
#include "../CosmeticEffects/ARActorCue.h"

#include "ParticleHelper.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

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
	DOREPLIFETIME(UARAttributeBaseComponent, ActiveEffects);

	DOREPLIFETIME(UARAttributeBaseComponent, ModifiedAttribute);
	DOREPLIFETIME_CONDITION(UARAttributeBaseComponent, ChangedAttribute, COND_OwnerOnly);
}
void UARAttributeBaseComponent::OnRep_ModifiedAttribute()
{
	GetAttributeModified();
}
void UARAttributeBaseComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*
	Client only.  When client get replicaated version of ActivePeriodicEffects, we run simulation on it.
	so we don't need to get timing from server each time.
	*/
	if (GetOwnerRole() < ROLE_Authority || GetNetMode() != ENetMode::NM_DedicatedServer )
	{
		for (auto It = ActiveEffects.Effects.CreateIterator(); It; ++It)
		{
			if (ActiveEffects.Effects[It.GetIndex()].IsActive)
			{
				ActiveEffects.Effects[It.GetIndex()].CurrentDuration += DeltaTime;
				if (!ActiveEffects.Effects[It.GetIndex()].IsActive)
					ActiveEffects.Effects[It.GetIndex()].CurrentDuration = 0;
			}
		}
		for (auto It = ActivePeriodicEffects.ActiveEffects.CreateIterator(); It; ++It)
		{
			ActivePeriodicEffects.ActiveEffects[It.GetIndex()].ClientCurrentDuration += DeltaTime;
			if (ActivePeriodicEffects.ActiveEffects[It.GetIndex()].ClientCurrentDuration >= ActivePeriodicEffects.ActiveEffects[It.GetIndex()].MaxDuration)
			{
				ActivePeriodicEffects.ActiveEffects[It.GetIndex()].ClientCurrentDuration = 0;
				ActivePeriodicEffects.ActiveEffects[It.GetIndex()].IsEffectActive = false;
			}
		}
	}
	//so we tick effects in struct. Maybe it's not best idea, but it's the only one I have right now...
	//we need to tick them on client for cosmetic as well
	if (GetOwnerRole() == ROLE_Authority)
	{
		for (auto It = ActiveEffects.Effects.CreateIterator(); It; ++It)
		{
			ActiveEffects.Effects[It.GetIndex()].ActorEffect->TickMe(DeltaTime);
		}
		for (auto It = ActivePeriodicEffects.ActiveEffects.CreateIterator(); It; ++It)
		{
			ActivePeriodicEffects.ActiveEffects[It.GetIndex()].PeriodicEffect->TickMe(DeltaTime);
		}
	}
}
void UARAttributeBaseComponent::ApplyPeriodicEffect(FEffectSpec& EffectIn)
{
	ActiveEffects.Effects.Add(EffectIn);
	EffectIn.IsActive = true;
}
void UARAttributeBaseComponent::OnRep_EffectAppiled()
{
	//apply cosmetic effect for effect. Somehow.
	//and remove it.
}

void UARAttributeBaseComponent::RemovedPeriodicEffect_TEMP(UAREffectPeriodicO* EffectIn)
{
	//for (auto It = ActiveEffects.Effects.CreateIterator(); It; ++It)
	//{
	//	if (ActiveEffects.Effects[It.GetIndex()].Effect == EffectIn)
	//	{
	//		ActiveEffects.Effects[It.GetIndex()].IsActive = false;
	//		//ActiveEffects.Effects[It.GetIndex()].CurrentDuration += DeltaTime;
	//	}
	//}
}
void UARAttributeBaseComponent::AddPeriodicEffect(FEffectSpec& PeriodicEffect)
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
		PeriodicEffect.ActorEffect->Initialze();
		ActiveEffects.Effects.Add(PeriodicEffect);
		PeriodicEffect.IsActive = true;
		AttachEffectCue(PeriodicEffect);
		OnPeriodicEffectAppiled.Broadcast(PeriodicEffect.OwnedTags);
	}

}

void UARAttributeBaseComponent::AttachEffectCue_Implementation(FEffectSpec EffectIn)
{
	AARCharacter* MyChar = Cast<AARCharacter>(GetOwner());
	if (!MyChar && EffectIn.EffectCue.ParticleSystem.IsValid())
		return;
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoCollisionFail = true;
	SpawnInfo.Owner = MyChar;
	AARActorCue* cue = GetWorld()->SpawnActor<AARActorCue>(AARActorCue::StaticClass(), SpawnInfo);
	if (!cue)
		return;
	cue->ParticleSystem = EffectIn.EffectCue.ParticleSystem;
	cue->InitializeAttachment(MyChar, "LeftHandSocket");
	cue->AttachRootComponentToActor(MyChar, "LeftHandSocket", EAttachLocation::SnapToTarget);

	//MyChar->PresistentParticle = UGameplayStatics::SpawnEmitterAttached(EffectIn.EffectCue.ParticleSystem.Get(), MyChar->Mesh, "LeftHandSocket", FVector(0, 0, 0), FRotator(0, 0, 0), EAttachLocation::Type::SnapToTarget);
}
void UARAttributeBaseComponent::DetachEffectCue_Implementation(FEffectSpec EffectIn)
{
	AARCharacter* MyChar = Cast<AARCharacter>(GetOwner());
	if (!MyChar && EffectIn.EffectCue.ParticleSystem.IsValid())
		return;

	TArray<AActor*> AttachedActors;
	MyChar->GetAttachedActors(AttachedActors);
	if (AttachedActors.Num() > 0)
	{
		for (AActor* actor : AttachedActors)
		{
			actor->Destroy();
		}
	}
	//if (!MyChar->PresistentParticle)
	//	return;

	//MyChar->PresistentParticle->Deactivate();
}
void UARAttributeBaseComponent::RemovePeriodicEffect(class AAREffectPeriodic* PeriodicEffect)
{ 
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerRemovePeriodicEffect(PeriodicEffect);
	}
	else
	{
		for (auto It = ActiveEffects.Effects.CreateIterator(); It; It++)
		{
			if (ActiveEffects.Effects[It.GetIndex()].ActorEffect == PeriodicEffect)
			{
				ActiveEffects.Effects[It.GetIndex()].IsActive = false;
				ActiveEffects.Effects[It.GetIndex()].ActorEffect->Destroy();
				DetachEffectCue(ActiveEffects.Effects[It.GetIndex()]);
				OnPeriodicEffectRemoved.Broadcast(ActiveEffects.Effects[It.GetIndex()].OwnedTags);
				ActiveEffects.Effects.RemoveAtSwap(It.GetIndex());
				return;
			}
		}
	}
}

void UARAttributeBaseComponent::ServerRemovePeriodicEffect_Implementation(class AAREffectPeriodic* PeriodicEffect)
{
	RemovePeriodicEffect(PeriodicEffect);
}

bool UARAttributeBaseComponent::ServerRemovePeriodicEffect_Validate(class AAREffectPeriodic* PeriodicEffect)
{
	return true;
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


//this could be split into multiple functions actially.
//maybe that would not be OOP, but could be cleaner and do not invovlve casting.
void UARAttributeBaseComponent::DamageAttribute(FARDamageEvent const& DamageEvent, AActor* EventInstigator, AActor* DamageCauser)
{
	if (DamageEvent.IsOfType(FARDamageEvent::ClassID))
	{
		SetFloatValue(AttributeOp(DamageEvent.Attribute.ModValue, GetFloatValue(DamageEvent.Attribute.AttributeName), DamageEvent.Attribute.OperationType), DamageEvent.Attribute.AttributeName);
		UDamageType* Damage = nullptr;
		if (DamageEvent.DamageTypeClass)
		{
			Damage = ConstructObject<UDamageType>(DamageEvent.DamageTypeClass);
		}
		UARAttributeBaseComponent* attr = EventInstigator->FindComponentByClass<UARAttributeBaseComponent>();

		if (attr)
		{
			attr->InstigatorAttributeDamageCaused(DamageEvent.Attribute, GetOwner(), DamageCauser, EventInstigator, Damage, DamageEvent.DamageTag);
		}
		SetAttributeChange(DamageEvent.Attribute, GetOwner(), DamageCauser, EventInstigator, Damage, DamageEvent.DamageTag);
		OnAttributeDamage.Broadcast(ChangedAttribute, DamageEvent.DamageTag);
	}
	else if (DamageEvent.IsOfType(FARPointDamageEvent::ClassID))
	{
		FARPointDamageEvent* const Point = (FARPointDamageEvent*)&DamageEvent;
		SetFloatValue(AttributeOp(Point->Attribute.ModValue, GetFloatValue(Point->Attribute.AttributeName), Point->Attribute.OperationType), Point->Attribute.AttributeName);
		UDamageType* Damage = nullptr;
		if (Point->DamageTypeClass)
		{
			Damage = ConstructObject<UDamageType>(Point->DamageTypeClass);
		}
		UARAttributeBaseComponent* attr = EventInstigator->FindComponentByClass<UARAttributeBaseComponent>();

		if (attr)
		{
			attr->InstigatorAttributeDamageCaused(Point->Attribute, GetOwner(), DamageCauser, EventInstigator, Damage, Point->DamageTag);
		}
		SetAttributeChange(Point->Attribute, GetOwner(), DamageCauser, EventInstigator, Damage, Point->DamageTag);
		OnAttributeDamage.Broadcast(ChangedAttribute, Point->DamageTag);
		OnPointAttributeDamage.Broadcast(Point->Attribute, EventInstigator, Point->HitInfo.ImpactPoint, Point->HitInfo.Component.Get(), Point->HitInfo.BoneName, Point->ShotDirection, Damage, DamageCauser);
	}
}
void UARAttributeBaseComponent::HealAttribute()
{

}

void UARAttributeBaseComponent::ChangeAttribute(FName AttributeName, float ModValue, TEnumAsByte<EAttrOp> OperationType)
{
	SetFloatValue(AttributeOp(ModValue, GetFloatValue(AttributeName), OperationType), AttributeName);
}

void UARAttributeBaseComponent::InstigatorAttributeDamageCaused(FAttribute Attribute, AActor* DamageTarget, AActor* DamageCauser, AActor* Instigator, UDamageType* DamageType, FGameplayTagContainer DamageTag)
{
	ChangedAttribute.Attribute = Attribute;
	ChangedAttribute.DamageCauser = DamageCauser;
	ChangedAttribute.DamageTarget = DamageTarget;
	//ChangedAttribute.ChangeInstigator = Instigator;
	ChangedAttribute.DamageType = DamageType;

	OnInstigatorCausedDamage.Broadcast(ChangedAttribute, DamageTag);
}

void UARAttributeBaseComponent::SetAttributeChange(FAttribute Attribute, AActor* DamageTarget, AActor* DamageCauser, AActor* Instigator, UDamageType* DamageType, FGameplayTagContainer DamageTag)
{
	ChangedAttribute.Attribute = Attribute;
	ChangedAttribute.DamageCauser = DamageCauser;
	ChangedAttribute.DamageTarget = DamageTarget;
	//ChangedAttribute.ChangeInstigator = Instigator;
	ChangedAttribute.DamageType = DamageType;
}

void UARAttributeBaseComponent::SetAttributeModified(float ModValue, FName AttributeName)
{
	//FAttributeModified ModedAttribute;
	//ModedAttribute.ActuallDamage = ModValue;
	//ModedAttribute.AttributeName = AttributeName;
	//OnAttributeModified.Broadcast(ModedAttribute);
}

void UARAttributeBaseComponent::GetAttributeModified()
{
	//OnAttributeModified.Broadcast(ModifiedAttribute);
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

