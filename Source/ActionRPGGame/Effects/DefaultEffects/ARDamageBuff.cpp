// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"
#include "../../ARPlayerController.h"
#include "../../Abilities/ARAbility.h"
#include "../../Componenets/AREquipmentComponent.h"
#include "../../Componenets/ARAttributeComponent.h"
#include "../../Items//ARWeapon.h"

#include "ARDamageBuff.h"

UARDamageBuff::UARDamageBuff(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}

void UARDamageBuff::Initialize()
{
	Super::Initialize();

	if (EffectInstigator.IsValid())
	{
		//UAREquipmentComponent* comp = EffectInstigator->FindComponentByClass<UAREquipmentComponent>();
		//if (comp)
		//{
		//	comp->OnRightWeaponActive.AddDynamic(this, &UARDamageBuff::OnRightWeaponEquiped);
		//	//if (comp->ActiveLeftHandWeapon)
		//	//{
		//		//comp->ActiveLeftHandWeapon->OnWeaponFired.AddDynamic(this, &UARDamageBuff::IncreaseDamage);
		//	//}
		//	//if (comp->ActiveLeftHandWeapon)
		//	//{
		//		//comp->ActiveLeftHandWeapon->OnWeaponFired.AddDynamic(this, &UARDamageBuff::IncreaseDamage);
		//	//}
		//}

		UARAttributeComponent* attrComp = EffectInstigator->FindComponentByClass<UARAttributeComponent>();

		attrComp->OnOutgoingDamage.AddDynamic(this, &UARDamageBuff::ModifyDamage);
		attrComp->OnIncomingDamage.AddDynamic(this, &UARDamageBuff::ReduceDamage);
		float objectSize = sizeof(*this);
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Black, FString::FormatAsNumber(objectSize));
	}
}

void UARDamageBuff::ModifyDamage_Implementation(const FGameplayTagContainer& DamageTags, const FAttribute& Attribute, class UARAttributeBaseComponent* Target)
{
	Target->DamageValueCached.ModValue += 3;
}

void UARDamageBuff::ReduceDamage_Implementation(const FGameplayTagContainer& DamageTags, const FAttribute& Attribute, class UARAttributeBaseComponent* Target)
{
	Target->DamageValueCached.ModValue -= 1;
}

void UARDamageBuff::OnRightWeaponEquiped(class AARWeapon* WeaponIn)
{
	if (WeaponIn)
	{
		for (FAttribute& attr : WeaponIn->DamageToApply)
		{
			attr.ModValue += 5;
		}
	}
}