// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "Net/UnrealNetwork.h"
#include "../ARCharacter.h"
#include "../ActionState/ARActionStateComponent.h"

#include "../BlueprintLibrary/ARTraceStatics.h"
#include "../../TraceComponent/ARTraceComponent.h"

#include "../../Effects/ARFeatComponent.h"

#include "ParticleHelper.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

#include "ARRangedWeapon.h"

AARRangedWeapon::AARRangedWeapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bNetUseOwnerRelevancy = true;
	bReplicateInstigator = true;
	bReplicates = true;

	WeaponState->SetRefireTime(RefireCooldown);
	WeaponState->SetIntervalTime(RefireSpeed);
	WeaponState->SetCooldownTime(ReloadTime);

}
void AARRangedWeapon::BeginPlay()
{
	Super::BeginPlay();
	/*
		This will need some rework, for weapon, where you load bullets one by one.
		Like shortgun or rifle.
		In that case cooldown time is "Ammount of bullets" * "Time to Load Single Bullet"
		Since there might be different amount of bullets to load, reload time will vary.
	*/
	//if (WeaponState->CooldownMontage)
	//{
	//	if (WeaponState->CooldownMontage->SequenceLength > RefireCooldown)
	//		WeaponState->SetRefireTime(WeaponState->CooldownMontage->SequenceLength);
	//	else
	//		WeaponState->SetRefireTime(RefireCooldown);
	//}
	//else
	//	WeaponState->SetRefireTime(RefireCooldown);

	//WeaponState->SetIntervalTime(RefireSpeed);
	//WeaponState->SetCooldownTime(ReloadTime);
	//if (Role >= ROLE_Authority)
	//{
	//	WeaponState->OnActionInterval.AddDynamic(this, &AARRangedWeapon::OnFiring);
	//	CurrentAmmo = MaxAmmo;
	//	CurrentMagazineSize = MaxMagazineSize;
	//}
}
/** IIARTracing override begin */
AARCharacter* AARRangedWeapon::GetCharacter()
{
	return ARCharacterOwner;
}
AARPlayerController* AARRangedWeapon::GetController()
{
	return ARPCOwner;
}
/* IIARTracing override begin **/

void AARRangedWeapon::InputPressed()
{
	StartFire();
}


void AARRangedWeapon::InputReleased()
{
	StopFire();
}

void AARRangedWeapon::ActionReload()
{
	WeaponState->GotoState(WeaponState->CooldownState);
	ReloadMagainze();
}

FVector AARRangedWeapon::GetOriginLocation()
{
	return UARTraceStatics::GetStartLocation(MuzzleSocket, ARCharacterOwner, WeaponHand);
}

void AARRangedWeapon::StartFire()
{
	if (Role < ROLE_Authority)
	{
		ServerStartFire(); 
	}
	else
	{
		Execute_ServerOnActionStart(this);
		WeaponState->StartAction();
		//exectute overrided blueprint functions,
		//to determine damage.
	//	DamageDeal->FinalDamage = ModifyDamageByAttribute(Damage);
		ModifyDamageByEffects(Damage);

		//DamageDeal->CheckEffects.Broadcast(DamageDeal);
		//Effect->CheckEffects.Broadcast(Damage);

	//	GEngine->AddOnScreenDebugMessage(0, 5, FColor::Red, FString::FormatAsNumber(damgeTest));

		if (!WeaponState->IsRecharing)
		{
			SubtractAmmo();
			if (CheckAmmo())
			{
				WeaponState->StartAction();
			}
		}
	}
}
void AARRangedWeapon::ModifyDamageByEffects_Implementation(float DamageIn)
{
	FAttribute Attr;
	//ARCharacterOwner->Feats->CachedDamage = DamageIn;
	Attr.ModValue = DamageIn;
	//float TestDamage;
	if (ARCharacterOwner->Feats->OnOutgoingDamage.IsBound())
	{
		ARCharacterOwner->Feats->OnOutgoingDamage.Broadcast(DamageIn, this);
	}
	GEngine->AddOnScreenDebugMessage(0, 4, FColor::Red, FString::FormatAsNumber(Damage));
}
void AARRangedWeapon::ServerStartFire_Implementation()
{
	StartFire();
}
bool AARRangedWeapon::ServerStartFire_Validate()
{
	return true;
}

void AARRangedWeapon::StopFire()
{
	WeaponState->StopAction();
	WeaponState->CastEnd();
}

/*
	Server
*/
bool AARRangedWeapon::CheckAmmo()
{
	if (CurrentMagazineSize >= AmmoPerShot && CurrentMagazineSize > 0)
		return true;
	else 
		return false;
}
/*
	Server.
*/
void AARRangedWeapon::SubtractAmmo()
{
	if (CheckAmmo())
		CurrentMagazineSize -= AmmoPerShot;
}
/*
	IT will use simple model when we just transfer needed amount of ammo, from one place to noather.
	In future it might change, to actually replacing magazines, so when magazine is no empty
	when reloading, ammo in it, is lost forever.

	Actually I might just add both, and use simple bool, or enum to switch between them.
*/
/*
	Server.
*/
void AARRangedWeapon::ReloadMagainze()
{
	if (Role < ROLE_Authority)
	{
		ServerReloadMagazine();
	}
	else
	{
		if (CurrentAmmo > 0)
		{
			int32 AmmoToReload = MaxMagazineSize - CurrentMagazineSize;

			if (CurrentAmmo >= AmmoToReload)
			{
				CurrentAmmo -= AmmoToReload;
				CurrentMagazineSize += AmmoToReload;
			}
			else
			{
				CurrentAmmo = 0;
				CurrentMagazineSize += CurrentAmmo;
			}
		}
	}
}

void AARRangedWeapon::ServerReloadMagazine_Implementation()
{
	ReloadMagainze();
}

bool AARRangedWeapon::ServerReloadMagazine_Validate()
{
	return true;
}

void AARRangedWeapon::OnFiring()
{
	SubtractAmmo();
	if (CheckAmmo())
	{
		
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Red, FString("No Ammo left!"));
		StopFire();
		return;
	}
}