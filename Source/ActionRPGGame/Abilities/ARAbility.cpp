// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../ActionState/ARActionStateComponent.h"
#include "../Componenets/ARAttributeBaseComponent.h"
#include "../Componenets/ARAttributeComponent.h"

#include "../Types/ARStructTypes.h"
#include "../ARCharacter.h"
#include "../Items/ARWeapon.h"
#include "../Componenets/AREquipmentComponent.h"
#include "../Items/ARWeapon.h"

#include "../BlueprintLibrary/ARTraceStatics.h"

#include "Net/UnrealNetwork.h"

#include "ARAbility.h"

AARAbility::AARAbility(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	MustHaveTwoWeaponActive = false;
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
	ActionState->SetCooldownTime(RechargeTime);


	LeftHandSocket = "LeftHandSocket";
	RightHandSocket = "RightHandSocket";
	WeaponSocket = "WeaponSocket";

	CurrentWeapon = nullptr;
	LeftWeapon = nullptr;
	RightWeapon = nullptr;
}
void AARAbility::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARAbility, CastingSpeed);
	DOREPLIFETIME(AARAbility, OwningCharacter);
	DOREPLIFETIME(AARAbility, CurrentWeapon);
}

void AARAbility::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	ActionState->TickMe(DeltaSeconds*CastingSpeed);

	/*
		This is only for cosmetic client side. It will also fire up action events on client (assuming they are bound).
		This will help with other systems, like spawning effects on client AND server.

		I probabaly should not do it inside tick.
	*/
	if (IsBeingUsed)
	{
		CurrentCastTime += DeltaSeconds*CastingSpeed;
		if (CurrentCastTime >= ActionState->MaxCastTime)
		{
			CurrentCastTime = 0;
			IsBeingUsed = false;
			IsOnCooldown = true;
			
		}
	}
	if (IsOnCooldown && ActionState->IsRecharing)
	{
		CurrentCooldownTime += DeltaSeconds*CastingSpeed;
		if (CurrentCooldownTime >= ActionState->ActionCooldownTime)
		{
			//IsOnCooldown = false;
		//	CurrentCooldownTime = 0;
		}
	}
	if (!ActionState->IsRecharing)
	{
		//IsOnCooldown = false;
		CurrentCooldownTime = 0;
	}

}

void AARAbility::Initialize()
{
	Execute_OnActionPrepared(this);
	OwnerEquipment = OwningCharacter->Equipment.Get();
	OwnerAttributes = OwningCharacter->Attributes.Get();

	OwnerEquipment->OnLeftWeaponActive.AddDynamic(this, &AARAbility::OnLeftHandWeapon);
	OwnerEquipment->OnRightWeaponActive.AddDynamic(this, &AARAbility::OnRightHandWeapon);
	LeftWeapon = OwnerEquipment->ActiveLeftHandWeapon;
	RightWeapon = OwnerEquipment->ActiveRightHandWeapon;

	ActionState->Owner = OwningCharacter;
}

void AARAbility::BeginPlay()
{
	Super::BeginPlay();
	ActionState->SetNetAddressable();
	ActionState->SetIsReplicated(true);
}
void AARAbility::OnLeftHandWeapon(class AARWeapon* WeaponIn)
{
	LeftWeapon = WeaponIn;
}
void AARAbility::OnRightHandWeapon(class AARWeapon* WeaponIn)
{
	RightWeapon = WeaponIn;
}
FVector AARAbility::GetOriginLocation()
{
	FVector Origin = FVector::ZeroVector;
	if (StartLocation == EAbilityOrigin::OpositeHandToWeapon)
	{
		if (CurrentWeapon->WeaponHand == EWeaponHand::WeaponLeft)
		{
			return UARTraceStatics::GetStartLocationFromCharacter(RightHandSocket, OwningCharacter, EWeaponHand::NoWeapon);
		}
		else if (CurrentWeapon->WeaponHand == EWeaponHand::WeaponRight)
		{
			return UARTraceStatics::GetStartLocationFromCharacter(LeftHandSocket, OwningCharacter, EWeaponHand::NoWeapon);
		}
	}
	else if (StartLocation == EAbilityOrigin::UseWeaponLocation)
	{
		if (!CurrentWeapon)
			return Origin;

		return UARTraceStatics::GetStartLocationFromCharacter(WeaponSocket, OwningCharacter, CurrentWeapon->WeaponHand);
	}

	return Origin;
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
	SetCurrentWeapon(OwningCharacter->Equipment->ActiveLeftHandWeapon);
	//this is for client side prediction.
	//only cosmetic stuff.

	if (GetNetMode() == ENetMode::NM_Standalone)
		return;

	if (!IsOnCooldown)
	{
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
}
void AARAbility::InputReleased()
{

}

//Executes On Server.
void AARAbility::StartAction()
{
	// 1. We need to check if player have right weapon equiped. - done
	// 2. We need to check if player have weapon at all for that matter ;). - done
	//    Abilities can't be used without weapon. Even bare hands are weapon in context of system.
	// 3. Then we need to check if player have resources needed to fire ability. - done
	// 4. We probably want to do it on client and on server. 
	// 5. On client because player can get message quicker than waiting for server to response.
	// 6. Though server have full authority.
	SetCurrentWeapon(OwningCharacter->Equipment->ActiveLeftHandWeapon);
	if (!ActionState->IsRecharing)
	{
		if (CheckWeapon())
		{
			if (CheckResources())
			{
				if (GetNetMode() == ENetMode::NM_Standalone)
					IsBeingUsed = true;
				Execute_ServerOnActionStart(this);
				ActionState->StartAction();
			}
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
	/*
		What if, ability require two weapon of specific type to be equiped ?
	*/

	if (!LeftWeapon && !RightWeapon)
		return false;

	if (LeftWeapon)
	{
		if (WeaponRequiredTags.MatchesAny(LeftWeapon->WeaponState->OwnedTags, false))
		{
			CurrentWeapon = LeftWeapon;
			return true;
		}
	}
	if (RightWeapon)
	{
		if (WeaponRequiredTags.MatchesAny(RightWeapon->WeaponState->OwnedTags, false))
		{
			CurrentWeapon = RightWeapon;
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