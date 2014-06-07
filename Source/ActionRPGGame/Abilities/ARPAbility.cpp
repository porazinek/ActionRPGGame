// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../ActionState/ARActionStateComponent.h"

#include "Net/UnrealNetwork.h"

#include "ARPAbility.h"

AARPAbility::AARPAbility(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	bReplicates = true;
	SetReplicates(true);

	bool IsCasting = false;
	CurrentCastTime = false;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;

	ActionState = PCIP.CreateDefaultSubobject<UARActionStateComponent>(this, TEXT("ActionState"));

	ActionState->SetNetAddressable();
	ActionState->SetIsReplicated(true);
}
void AARPAbility::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARPAbility, BlankRep);
}

void AARPAbility::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	ActionState->TickMe(DeltaSeconds);

	//This part of tick is for client
	if (IsBeingUsed)
	{
		CurrentCastTime += DeltaSeconds;
		if (CurrentCastTime >= ActionState->MaxCastTime)
		{
			CurrentCastTime = 0;
			IsBeingUsed = false;
			IsOnCooldown = true;
			
		}
	}
	if (IsOnCooldown)
	{
		CurrentCooldownTime += DeltaSeconds;
		if (CurrentCooldownTime >= ActionState->ActionCooldownTime)
		{
			IsOnCooldown = false;
			CurrentCooldownTime = 0;
			PrimaryActorTick.SetTickFunctionEnable(false);
			PrimaryActorTick.UnRegisterTickFunction();
		}
	}

}

void AARPAbility::InputPressed()
{
	//this is for client side prediction.
	//only cosmetic stuff.
	if (!IsOnCooldown)
	{
		IsBeingUsed = true;
		PrimaryActorTick.SetTickFunctionEnable(true);
		PrimaryActorTick.RegisterTickFunction(GetLevel());
	}
	//PrimaryActorTick.bStartWithTickEnabled = true;
	if (Role < ROLE_Authority)
	{
		ServerStartAction();
	}
	else
	{
		StartAction(); //we are on server, we just call normal version.
	}
}
void AARPAbility::StartAction()
{
	PrimaryActorTick.SetTickFunctionEnable(true);
	PrimaryActorTick.RegisterTickFunction(GetLevel());
	Execute_OnActionStart(this);
	ActionState->StartAction();
}

void AARPAbility::ServerStartAction_Implementation()
{
	StartAction();
}

bool AARPAbility::ServerStartAction_Validate()
{
	return true;
}