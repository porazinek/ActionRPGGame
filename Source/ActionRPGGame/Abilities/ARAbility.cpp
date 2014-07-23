// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../ActionState/ARActionStateComponent.h"
#include "../Componenets/ARAttributeBaseComponent.h"
#include "../Componenets/ARAttributeComponent.h"

#include "../Types/ARStructTypes.h"
#include "../ARCharacter.h"
#include "../Items/ARWeapon.h"
#include "../Componenets/AREquipmentComponent.h"

#include "Net/UnrealNetwork.h"

#include "ARAbility.h"

AARAbility::AARAbility(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	bReplicates = true;
	SetReplicates(true);
	CastingSpeed = 1;
	bool IsCasting = false;
	CurrentCastTime = 0;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;

	ActionState = PCIP.CreateDefaultSubobject<UARActionStateComponent>(this, TEXT("ActionState"));
	ActionState->SetMaxCastTime(MaxCastTime);
	ActionState->SetNetAddressable();
	ActionState->SetIsReplicated(true);
}
void AARAbility::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARAbility, BlankRep);
	DOREPLIFETIME(AARAbility, CastingSpeed);
	DOREPLIFETIME(AARAbility, OwningCharacter);
}

void AARAbility::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	ActionState->TickMe(DeltaSeconds*CastingSpeed);

	/*
		This is only for cosmetic client side. It will also fire up action events on client (assuming they are bound).
		This will help with other systems, like spawning effects on client AND server.
	*/
	if (IsBeingUsed)
	{
		CurrentCastTime += DeltaSeconds*CastingSpeed;
		if (CurrentCastTime >= ActionState->MaxCastTime)
		{
			//ActionState->FireAction();
			CurrentCastTime = 0;
			IsBeingUsed = false;
			IsOnCooldown = true;
			//ActionState->CooldownBegin();
			
		}
	}
	if (IsOnCooldown)
	{
		CurrentCooldownTime += DeltaSeconds*CastingSpeed;
		if (CurrentCooldownTime >= ActionState->ActionCooldownTime)
		{
			//ActionState->CooldownEnded();
			IsOnCooldown = false;
			CurrentCooldownTime = 0;
			//PrimaryActorTick.SetTickFunctionEnable(false);
			//PrimaryActorTick.UnRegisterTickFunction();
		}
	}

}

void AARAbility::Initialize()
{
	Execute_OnActionPrepared(this);
}

void AARAbility::InputPressed()
{

	//PrimaryActorTick.bStartWithTickEnabled = true;
	if (Role < ROLE_Authority)
	{
		ServerStartAction();
	}
	else
	{
		StartAction(); //we are on server, we just call normal version.
	}
	//this is for client side prediction.
	//only cosmetic stuff.
	if (CheckWeapon())
	{
		if (CheckResources())
		{
			if (!IsOnCooldown)
			{
				//Execute_ServerOnActionStart(this);
				//Execute_ClientOnActionStart(this);

				//if (Role < ROLE_Authority)
				//{
				//	ActionState->CastBegin();
				//}
				IsBeingUsed = true;
			}
		}
	}
}
void AARAbility::InputReleased()
{

}
void AARAbility::StartAction()
{
	// 1. We need to check if player have right weapon equiped. - done
	// 2. We need to check if player have weapon at all for that matter ;). - done
	//    Abilities can't be used without weapon. Even bare hands are weapon in context of system.
	// 3. Then we need to check if player have resources needed to fire ability. - done
	// 4. We probably want to do it on client and on server. 
	// 5. On client because player can get message quicker than waiting for server to response.
	// 6. Though server have full authority.
	if (CheckWeapon())
	{
		if (CheckResources())
		{
			Execute_ServerOnActionStart(this);
			ActionState->StartAction();
		}
	}
}
void AARAbility::ServerStartAction_Implementation()
{
	StartAction();
}
bool AARAbility::ServerStartAction_Validate()
{
	return true;
}

bool AARAbility::CheckWeapon()
{
	//To->many->de->references->must->be->fixed!
	/*
		Remove Equipment component ?
		Move Owned Tags from Component to Weapon.
		Profit ?
	*/
	if (!OwningCharacter->Equipment->ActiveLeftHandWeapon && !OwningCharacter->Equipment->ActiveRightHandWeapon)
		return false;

	if (OwningCharacter->Equipment->ActiveLeftHandWeapon)
	{
		if (WeaponRequiredTags.MatchesAny(OwningCharacter->Equipment->ActiveLeftHandWeapon->WeaponState->OwnedTags, false))
		{
			return true;
		}
	}
	if (OwningCharacter->Equipment->ActiveRightHandWeapon)
	{
		if (WeaponRequiredTags.MatchesAny(OwningCharacter->Equipment->ActiveRightHandWeapon->WeaponState->OwnedTags, false))
		{
			return true;
		}
	}
	return false;
}
bool AARAbility::CheckResources()
{
	for (FAttribute& Resource : ResourceCost)
	{
		if (OwningCharacter->Attributes->CompareAttributeValue(Resource.ModValue, Resource.AttributeName, ECompareAttribute::Attr_Bigger))
		{
			return false;
		}
	}

	for (FAttribute& Resource : ResourceCost)
	{
		OwningCharacter->Attributes->ChangeAttribute(Resource.AttributeName, Resource.ModValue, EAttrOp::Attr_Subtract);
	}
	//at this point all is valid and we can return true.
	return true;
}