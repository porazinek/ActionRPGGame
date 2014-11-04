// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../Types/ARStructTypes.h"
#include "../Types/AREnumTypes.h"
#include "../Types/ARAttributeTypes.h"

#include "../Effects/AREffectPeriodic.h"
#include "../ARCharacter.h"
#include "../CosmeticEffects/ARActorCue.h"
#include "../ARPlayerController.h"

#include "ParticleHelper.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

#include "../Attributes/IAttribute.h"

#include "../HUD/ARHUD.h"

#include "Net/UnrealNetwork.h"

#include "ARAttributeBaseComponent.h"

UARAttributeBaseComponent::UARAttributeBaseComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
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
	DOREPLIFETIME_CONDITION(UARAttributeBaseComponent, UIDamage, COND_OwnerOnly);
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
	if (GetOwnerRole() < ROLE_Authority || GetNetMode() != ENetMode::NM_DedicatedServer)
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

void UARAttributeBaseComponent::Initialize()
{

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

		PeriodicEffect.ActorEffect->Initialze();
		ActiveEffects.Effects.Add(PeriodicEffect);
		PeriodicEffect.IsActive = true;
		AttachEffectCue(PeriodicEffect);

		if (PeriodicEffect.ActorEffect.IsValid())
		{
			UARAttributeBaseComponent* causerCauserAttr = PeriodicEffect.ActorEffect->EffectCauser->FindComponentByClass<UARAttributeBaseComponent>();

			if (causerCauserAttr)
			{
				causerCauserAttr->OnPeriodicEffectInstigated.Broadcast(PeriodicEffect.OwnedTags);
			}
		}

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

	for (TSubclassOf<class AARActorCue> cue : EffectIn.ActorCues)
	{
		AARActorCue* cueObj = GetWorld()->SpawnActor<AARActorCue>(cue, SpawnInfo);
		if (!cueObj)
			return;
		cueObj->ParticleSystem = EffectIn.EffectCue.ParticleSystem;
		cueObj->InitializeAttachment(MyChar, EffectIn.EffectCue.AttachLocation);
		cueObj->AttachRootComponentTo(MyChar->GetMesh(), EffectIn.EffectCue.AttachLocation, EAttachLocation::SnapToTarget);
	}
}
void UARAttributeBaseComponent::DetachEffectCue_Implementation(FEffectSpec EffectIn)
{
	AARCharacter* MyChar = Cast<AARCharacter>(GetOwner());
	//if (!MyChar && EffectIn.EffectCue.ParticleSystem.IsValid())
	//	return;
	if (!MyChar)
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
			if (ActiveEffects.Effects[It.GetIndex()].ActorEffect.Get() == PeriodicEffect)
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
/*
	This function is begging me for clean up.
	It might be right actually.
	*/
void UARAttributeBaseComponent::DamageAttribute(FARDamageEvent const& DamageEvent, AActor* EventInstigator, AActor* DamageCauser)
{
	if (DamageEvent.IsOfType(FARDamageEvent::ClassID))
	{
		//and finally we apply it.
		//SetFloatValue(AttributeOp(DamageValueCached.ModValue, GetFloatValue(DamageValueCached.AttributeName), DamageValueCached.OperationType), DamageValueCached.AttributeName);
		SetFloatValue(AttributeOp(DamageEvent.Attribute.ModValue, GetFloatValue(DamageEvent.Attribute.AttributeName), DamageEvent.Attribute.OperationType), DamageEvent.Attribute.AttributeName);
		 
		//SetFloatValue(AttributeOp(DamageEvent.Attribute.ModValue, GetFloatValue(DamageEvent.Attribute.AttributeName), DamageEvent.Attribute.OperationType), DamageEvent.Attribute.AttributeName);
		UDamageType* Damage = nullptr;
		if (DamageEvent.DamageTypeClass)
		{
			Damage = ConstructObject<UDamageType>(DamageEvent.DamageTypeClass);
		}
		/*
			TODO!!!
			Part about setting up damage events and data is in need of serious rework!
			1. Better organize what data is send.
			2. Add more specialized function for Radial Damage
			3. Split Damage Info (hit location, radius, damage type) from what was damaged.
			*/

		//SetAttributeChange(DamageValueCached, DamageEvent.HitInfo.Location, GetOwner(), DamageCauser, EventInstigator, Damage, DamageEvent.DamageTag);
		//handle death.
		IIAttribute* attrInt = Cast<IIAttribute>(GetOwner());
		if (attrInt)
		{
			attrInt->Execute_OnRecivedDamage(GetOwner(), ChangedAttribute, DamageEvent, DamageEvent.DamageTag);
			if (DamageEvent.Attribute.AttributeName == attrInt->GetDeathAttribute())
			{
				if (GetFloatValue(DamageEvent.Attribute.AttributeName) <= 0 && DamageEvent.Attribute.ModValue > 0) //prolly want to move to interface as well. Different actor might want 
					//different thresholds.
				{

					attrInt->Died();
				}
			}
		}

		OnAttributeDamage.Broadcast(ChangedAttribute, DamageEvent.DamageTag);
	}
}
//void UARAttributeBaseComponent::DamageAttribute(FARDamageEvent const& DamageEvent, AActor* EventInstigator, AActor* DamageCauser)
//{
//	UARAttributeBaseComponent* attrChar = EventInstigator->FindComponentByClass<UARAttributeBaseComponent>();
//	if (attrChar)
//	{
//		/*
//			Fire even on Instigator, to set incoming base Damage to DamageValueCached on instigator.
//			Then any effects on instigator might modify it.
//		*/
//		attrChar->DamageValueCached = DamageEvent.Attribute;
//		attrChar->OnOutgoingDamage.Broadcast(DamageEvent.DamageTag, DamageEvent.Attribute, attrChar);
//	}
//	
//	/*
//	We should also apply other standard modifcation, which are going to appiled whther
//	both objects have feat or not, these modification might include:
//	1. Increased damage based on attribute, which appiled to all outgoing damage of matching type.
//	2. Damage reduction by Armor/Spell Resistance/Elemental Resitance/etc.
//
//	There are at least three ways by which those can be appiled:
//	1. Weapon/Ability will apply them before passing damage to Apply Damage function.
//	It have bad side, of the fact, where designer must remember to put those function in each
//	ability. For 10 or 20 abilities it easy enough, but for 200-300 ?
//	2. Variation of above. Designer still have to put something in the weapon/ability
//	but this time it is predefined effect, which will catch incoming data, modify it
//	and pass it to this function.
//	It can be useful, because each effect can modify incoming data in different ways,
//	so it easy enough to create lots of variations for different responses for different attributes
//	for each ability/weapon, and then just use them as you see fit.
//	Object are created only for life time needed to modify incoming data, and then immidietly marked
//	for garbage collection, and are quickly enough destroyed.
//	The downside is, it is not really that easy to use in blueprint, where you might be forced to
//	pass informations from one object to another.
//	3. For each player create list of effects (static list ?), which are always present
//	and always listen for events. When there is match, they just modify incoming value, and output
//	modified one, for other effects to apply mods for it.
//	*/
//
//	/*
//	Ok this is an interesting part, that I'm trying to resolve currently.
//
//	Essentialy DamageAttribute doesn't calculate any damage. It just take any incoming value and
//	modify by it attribute. In that case it subtract it.
//	Before we apply damage, we call OnInstigatorCausedDamage delegate on Causer.
//	Now that delegate is inteded for use in two places:
//	1. Slate/UI to display damage done by player.
//	2. Effects. Before damage will be appiled, we should be able to modify in any other object.
//	In that case it will Feat object (UAREffectType base class), which can perform any operation
//	based on incoming DamageTags and value. And then send it back here, to be finally appiled.
//	It should work for causer (for example causer might have feat which will increase damage for
//	all one handed swords), and for affected Target (in the essence this component), for actor possing it
//	might have feat that will reduce all incoming physical damage.
//
//	After both interested objects had chance to modify incoming value, it should be appiled.
//	The order of operation is very important. Causer should always modify incoming value first,
//	as it is point of origin, and player expect that any mods from his character will be appiled first
//	when dealing damage.
//
//	Then any mods on target should appiled.
//	*/
//
//	/*
//		Copy modified damage from Instigator to our local variable.
//	*/
//	DamageValueCached = attrChar->DamageValueCached;
//
//	/*
//		Give chance to local effects to modify any incoming damage on this actor.
//	*/
//	OnIncomingDamage.Broadcast(DamageEvent.DamageTag, DamageValueCached, this);
//
//	// Normal single target Damage.
//	if (DamageEvent.IsOfType(FARDamageEvent::ClassID))
//	{
//		//and finally we apply it.
//		SetFloatValue(AttributeOp(DamageValueCached.ModValue, GetFloatValue(DamageValueCached.AttributeName), DamageValueCached.OperationType), DamageValueCached.AttributeName);
//
//		//SetFloatValue(AttributeOp(DamageEvent.Attribute.ModValue, GetFloatValue(DamageEvent.Attribute.AttributeName), DamageEvent.Attribute.OperationType), DamageEvent.Attribute.AttributeName);
//		UDamageType* Damage = nullptr;
//		if (DamageEvent.DamageTypeClass)
//		{
//			Damage = ConstructObject<UDamageType>(DamageEvent.DamageTypeClass);
//		}
//		/*
//			TODO!!!
//			Part about setting up damage events and data is in need of serious rework!
//			1. Better organize what data is send.
//			2. Add more specialized function for Radial Damage
//			3. Split Damage Info (hit location, radius, damage type) from what was damaged.
//		*/
//
//		//SetAttributeChange(DamageValueCached, DamageEvent.HitInfo.Location, GetOwner(), DamageCauser, EventInstigator, Damage, DamageEvent.DamageTag);
//		//handle death.
//		IIAttribute* attrInt = InterfaceCast<IIAttribute>(GetOwner());
//		if (attrInt)
//		{
//			attrInt->Execute_OnRecivedDamage(GetOwner(), ChangedAttribute, DamageEvent, DamageEvent.DamageTag);
//			if (DamageEvent.Attribute.AttributeName == attrInt->GetDeathAttribute())
//			{
//				if (GetFloatValue(DamageEvent.Attribute.AttributeName) <= 0 && DamageEvent.Attribute.ModValue > 0) //prolly want to move to interface as well. Different actor might want 
//					//different thresholds.
//				{
//					
//					attrInt->Died();
//				}
//			}
//		}
//
//		if (attrChar)
//		{
//			SetDamageReplication(attrChar);
//		}
//		
//		OnAttributeDamage.Broadcast(ChangedAttribute, DamageEvent.DamageTag);
//	}
//	//Point Damage. I'm not sure if I even need this.
//	else if (DamageEvent.IsOfType(FARPointDamageEvent::ClassID))
//	{
//		FARPointDamageEvent* const Point = (FARPointDamageEvent*)&DamageEvent;
//		SetFloatValue(AttributeOp(Point->Attribute.ModValue, GetFloatValue(Point->Attribute.AttributeName), Point->Attribute.OperationType), Point->Attribute.AttributeName);
//		UDamageType* Damage = nullptr;
//		if (Point->DamageTypeClass)
//		{
//			Damage = ConstructObject<UDamageType>(Point->DamageTypeClass);
//		}
//		UARAttributeBaseComponent* attr = EventInstigator->FindComponentByClass<UARAttributeBaseComponent>();
//		SetAttributeChange(Point->Attribute, DamageEvent.HitInfo.Location, GetOwner(), DamageCauser, EventInstigator, Damage, Point->DamageTag);
//		
//		IIAttribute* attrInt = InterfaceCast<IIAttribute>(GetOwner());
//		if (attrInt)
//		{
//			attrInt->Execute_OnRecivedDamage(GetOwner(), ChangedAttribute, DamageEvent, DamageEvent.DamageTag);
//			if (DamageEvent.Attribute.AttributeName == attrInt->GetDeathAttribute())
//			{
//				if (GetFloatValue(DamageEvent.Attribute.AttributeName) <= 0 && DamageEvent.Attribute.ModValue > 0) //prolly want to move to interface as well. Different actor might want 
//					//different thresholds.
//				{
//					attrInt->Died();
//				}
//			}
//		}
//
//		if (attr)
//		{
//			attr->InstigatorAttributeDamageCaused(Point->Attribute, DamageEvent.HitInfo.Location, GetOwner(), DamageCauser, EventInstigator, Damage, Point->DamageTag);
//			//attr->OnInstigatorCausedDamage.Broadcast(attr->ChangedAttribute, DamageEvent.DamageTag, attr->ChangedAttribute.Attribute);
//		}
//
//		OnAttributeDamage.Broadcast(ChangedAttribute, Point->DamageTag);
//		OnPointAttributeDamage.Broadcast(Point->Attribute, EventInstigator, Point->HitInfo.ImpactPoint, Point->HitInfo.Component.Get(), Point->HitInfo.BoneName, Point->ShotDirection, Damage, DamageCauser);
//	}
//	//Radial Damage.
//	else if (DamageEvent.IsOfType(FARRadialDamageEvent::ClassID))
//	{
//		FARRadialDamageEvent* const Point = (FARRadialDamageEvent*)&DamageEvent;
//		SetFloatValue(AttributeOp(Point->Attribute.ModValue, GetFloatValue(Point->Attribute.AttributeName), Point->Attribute.OperationType), Point->Attribute.AttributeName);
//		UDamageType* Damage = nullptr;
//		if (Point->DamageTypeClass)
//		{
//			Damage = ConstructObject<UDamageType>(Point->DamageTypeClass);
//		}
//		UARAttributeBaseComponent* attr = EventInstigator->FindComponentByClass<UARAttributeBaseComponent>();
//		SetAttributeChange(Point->Attribute, DamageEvent.HitInfo.Location, GetOwner(), DamageCauser, EventInstigator, Damage, Point->DamageTag);
//
//		IIAttribute* attrInt = InterfaceCast<IIAttribute>(GetOwner());
//		if (attrInt)
//		{
//			attrInt->Execute_OnRecivedRadialDamage(GetOwner(), ChangedAttribute, *Point, DamageEvent.DamageTag);
//			attrInt->Execute_OnRecivedDamage(GetOwner(), ChangedAttribute, DamageEvent, DamageEvent.DamageTag);
//			if (DamageEvent.Attribute.AttributeName == attrInt->GetDeathAttribute())
//			{
//				if (GetFloatValue(DamageEvent.Attribute.AttributeName) <= 0 && DamageEvent.Attribute.ModValue > 0) //prolly want to move to interface as well. Different actor might want 
//					//different thresholds.
//				{
//					attrInt->Died();
//				}
//			}
//		}
//
//		if (attr)
//		{
//			attr->InstigatorAttributeDamageCaused(Point->Attribute, DamageEvent.HitInfo.Location, GetOwner(), DamageCauser, EventInstigator, Damage, Point->DamageTag);
//		}
//
//		OnAttributeDamage.Broadcast(ChangedAttribute, Point->DamageTag);
//		
//		//OnPointAttributeDamage.Broadcast(Point->Attribute, EventInstigator, Point->HitInfo.ImpactPoint, Point->HitInfo.Component.Get(), Point->HitInfo.BoneName, Point->ShotDirection, Damage, DamageCauser);
//	}
//	//Add:
//	//1. Square Ddamage (essentially aoe, but can be narrow like recangle or actually square). (in line).
//}

void UARAttributeBaseComponent::HealAttribute(FARHealEvent const& HealEvent, AActor* EventInstigator, AActor* DamageCauser)
{
	UARAttributeBaseComponent* attrChar = EventInstigator->FindComponentByClass<UARAttributeBaseComponent>();
	if (attrChar)
	{
		/*
		Fire even on Instigator, to set incoming base Damage to DamageValueCached on instigator.
		Then any effects on instigator might modify it.
		*/
		attrChar->HealValueCached = HealEvent.Attribute;
		attrChar->OnOutgoingHeal.Broadcast(HealEvent.HealTag, HealEvent.Attribute, attrChar);
	}

	/*
		Copy modified damage from Instigator to our local variable.
		*/
	HealValueCached = attrChar->HealValueCached;

	/*
		Give chance to local effects to modify any incoming damage on this actor.
		*/
	OnIncomingHeal.Broadcast(HealEvent.HealTag, HealValueCached, this);

	// Normal single target Damage.
	if (HealEvent.IsOfType(FARHealEvent::ClassID))
	{
		//and finally we apply it.
		SetFloatValue(AttributeOp(DamageValueCached.ModValue, GetFloatValue(DamageValueCached.AttributeName), EAttrOp::Attr_Add), DamageValueCached.AttributeName);

		//SetFloatValue(AttributeOp(DamageEvent.Attribute.ModValue, GetFloatValue(DamageEvent.Attribute.AttributeName), DamageEvent.Attribute.OperationType), DamageEvent.Attribute.AttributeName);
		/*
		TODO!!!
		Part about setting up damage events and data is in need of serious rework!
		1. Better organize what data is send.
		2. Add more specialized function for Radial Damage
		3. Split Damage Info (hit location, radius, damage type) from what was damaged.
		*/

		if (attrChar)
		{

		}

		//	OnAttributeDamage.Broadcast(ChangedAttribute, DamageEvent.DamageTag);
	}
}

void UARAttributeBaseComponent::SetDamageReplication(UARAttributeBaseComponent* Comp)
{
	//Comp->InstigatorAttributeDamageCaused(DamageValueCached, DamageEvent.HitInfo.Location, GetOwner(), DamageCauser, EventInstigator, Damage, DamageEvent.DamageTag);
	//Set for display on Instigator UI.
	Comp->UIDamage.Value = DamageValueCached.ModValue;
	Comp->UIDamage.Location = GetOwner()->GetActorLocation();
	Comp->UIDamage.Location += FVector(FMath::FRandRange(-2, 2), FMath::FRandRange(-2, 2), FMath::FRandRange(-2, 2));
	Comp->UIDamage.DamageInstigator = Comp->GetOwner();

	Comp->PlayerController->UIDamage.Value = DamageValueCached.ModValue;
	Comp->PlayerController->UIDamage.Location = GetOwner()->GetActorLocation();
	Comp->PlayerController->UIDamage.Location += FVector(FMath::FRandRange(-2, 2), FMath::FRandRange(-2, 2), FMath::FRandRange(-2, 2));
	Comp->PlayerController->UIDamage.DamageInstigator = Comp->GetOwner();

	//Comp->PlayerController->UIDamage

	//we also might want to display on target UI.
	UIDamage.Value = DamageValueCached.ModValue;
	UIDamage.Location = GetOwner()->GetActorLocation();

	UIDamage.Location += FVector(FMath::FRandRange(-2, 2), FMath::FRandRange(-2, 2), FMath::FRandRange(-2, 2));
	UIDamage.DamageInstigator = Comp->GetOwner();

	Comp->OnInstigatorDamage.Broadcast(Comp->UIDamage);
}

void UARAttributeBaseComponent::OnRep_UIDamage()
{
	//OnInstigatorDamage.Broadcast();
	if (UIDamage.DamageInstigator)
	{
		TWeakObjectPtr<UARAttributeBaseComponent> instAttr = UIDamage.DamageInstigator->FindComponentByClass<UARAttributeBaseComponent>();
		if (instAttr.IsValid())
		{
			instAttr->OnInstigatorDamage.Broadcast(instAttr->UIDamage);
			//AARHUD* hud = Cast<AARHUD>(instAttr->PlayerController->GetHUD());
			//if (hud)
			//{
			//	hud->PawnDamaged(instAttr->UIDamage.Location, instAttr->UIDamage.Value, UDamageType::StaticClass());
			//}
		}
	}
}

void UARAttributeBaseComponent::ChangeAttribute(FName AttributeName, float ModValue, TEnumAsByte<EAttrOp> OperationType)
{
	SetFloatValue(AttributeOp(ModValue, GetFloatValue(AttributeName), OperationType), AttributeName);
}

bool UARAttributeBaseComponent::CompareAttributes(FName AttributeA, FName AttributeB, TEnumAsByte<ECompareAttribute::Type> Comparsion)
{
	float ValueA = GetFloatValue(AttributeA);
	float ValueB = GetFloatValue(AttributeB);

	switch (Comparsion)
	{
	case ECompareAttribute::Type::Attr_Bigger:
		if (ValueA > ValueB)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case ECompareAttribute::Type::Attr_BiggerEqual:
		if (ValueA >= ValueB)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case ECompareAttribute::Type::Attr_Equal:
		if (ValueA == ValueB)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case ECompareAttribute::Type::Attr_Smaller:
		if (ValueA < ValueB)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case ECompareAttribute::Type::Attr_SmallerEqual:
		if (ValueA <= ValueB)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	default:
		return false;
	}
	return false;
}

bool UARAttributeBaseComponent::CompareAttributeValue(float Value, FName Attribute, TEnumAsByte<ECompareAttribute::Type> Comparsion)
{
	float ValueB = GetFloatValue(Attribute);

	switch (Comparsion)
	{
	case ECompareAttribute::Type::Attr_Bigger:
		if (Value > ValueB)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case ECompareAttribute::Type::Attr_BiggerEqual:
		if (Value >= ValueB)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case ECompareAttribute::Type::Attr_Equal:
		if (Value == ValueB)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case ECompareAttribute::Type::Attr_Smaller:
		if (Value < ValueB)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case ECompareAttribute::Type::Attr_SmallerEqual:
		if (Value <= ValueB)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	default:
		return false;
	}
	return false;
}

void UARAttributeBaseComponent::InstigatorAttributeDamageCaused(FAttribute Attribute, FVector HitLocation, AActor* DamageTarget, AActor* DamageCauser, AActor* Instigator, UDamageType* DamageType, FGameplayTagContainer DamageTag)
{
	ChangedAttribute.Attribute = Attribute;
	ChangedAttribute.HitLocation = HitLocation;
	ChangedAttribute.DamageCauser = DamageCauser;
	ChangedAttribute.DamageTarget = DamageTarget;
	ChangedAttribute.DamageInstigator = Instigator;
	ChangedAttribute.DamageType = DamageType;

	//OnInstigatorCausedDamage.Broadcast(ChangedAttribute, DamageTag, Attribute);
}

void UARAttributeBaseComponent::OnRep_InstigatorDamageCaused()
{
	if (ChangedAttribute.DamageInstigator)
	{
		TWeakObjectPtr<UARAttributeBaseComponent> instAttr = ChangedAttribute.DamageInstigator->FindComponentByClass<UARAttributeBaseComponent>();
		if (instAttr.IsValid())
		{
			//instAttr->OnInstigatorDamage.Broadcast();
			instAttr->OnInstigatorCausedDamage.Broadcast(instAttr->ChangedAttribute);
		}
	}
}

void UARAttributeBaseComponent::SetAttributeChange(FAttribute Attribute, FVector HitLocation, AActor* DamageTarget, AActor* DamageCauser, AActor* Instigator, UDamageType* DamageType, FGameplayTagContainer DamageTag)
{
	ChangedAttribute.Attribute = Attribute;
	ChangedAttribute.HitLocation = HitLocation;
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

