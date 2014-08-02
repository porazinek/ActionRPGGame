// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "ARActionState.h"
#include "ARActionStateActive.h"
#include "ARACtionStateCooldown.h"
#include "ARActionStateCasting.h"
#include "../BlueprintLibrary/ARTraceStatics.h"
#include "../ARCharacter.h"

#include "IARActionState.h"

#include "Net/UnrealNetwork.h"

#include "ARActionStateComponent.h"

UARActionStateComponent::UARActionStateComponent(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	SetIsReplicated(true);
	bReplicates = true;
	bWantsInitializeComponent = true;
	PlayRechargeAnimation = false;
	IsCasting = false;
	IsRecharing = false;
	ActiveState = PCIP.CreateDefaultSubobject<UARActionStateActive>(this, TEXT("StateActive"));
	CooldownState = PCIP.CreateDefaultSubobject<UARActionStateCooldown>(this, TEXT("StateCooldown"));
}

void UARActionStateComponent::InitializeComponent()
{
	//hack there need to be another state.
	if (CurrentState == NULL)
	{
		GotoState(ActiveState);
	}
	if (ActionStateClass)
	{
		StartActionState = ConstructObject<UARActionStateCasting>(ActionStateClass, this);
	}
	checkSlow(CurrentState != NULL);
}

void UARActionStateComponent::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARActionStateComponent, IsCasting);
	DOREPLIFETIME(UARActionStateComponent, IsRecharing);
	DOREPLIFETIME(UARActionStateComponent, Owner);
}

void UARActionStateComponent::TickMe(float DeltaTime)
{
	if (CurrentState)
	{
		CurrentState->Tick(DeltaTime);
	}
}
void UARActionStateComponent::PostNetReceive()
{
	//if (IsCasting)
	//	ServerSetCastingState(false);
}
void UARActionStateComponent::PreNetReceive()
{
	//if (!IsCasting)
	//	ServerSetCastingState(true);
}
void UARActionStateComponent::GotoState(class UARActionState* NextState)
{
	if (NextState == NULL || !NextState->IsIn(this))
	{

	}
	else if (CurrentState != NextState)
	{
		UARActionState* PrevState = CurrentState;
		if (CurrentState != NULL)
		{
			CurrentState->EndState();
		}
		if (CurrentState == PrevState)
		{
			CurrentState = NextState;
			CurrentState->BeginState(PrevState);
		}
	}
}

void UARActionStateComponent::BeginActionSequence()
{
	if (CurrentState)
	{
		CurrentState->BeginActionSequence();
	}
}
void UARActionStateComponent::EndActionSequence()
{
	if (CurrentState)
	{
		CurrentState->EndActionSequence();
	}
}

void UARActionStateComponent::FireAction()
{
	CastEnd();
}

void UARActionStateComponent::StartAction()
{
	//BeginActionSequence();
	ENetRole rul = GetOwnerRole();
	if (rul < ROLE_Authority)
	{
		ServerStartAction();
		BeginActionSequence();
	}
	else
	{
		OnActionPreCast.Broadcast(OwnedTags);
		BeginActionSequence();
	}
}
void UARActionStateComponent::ServerStartAction_Implementation()
{
	BeginActionSequence();
}
bool UARActionStateComponent::ServerStartAction_Validate()
{
	return true;
}

void UARActionStateComponent::StopAction()
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerStopAction();
		EndActionSequence();
	}
	else
	{
		EndActionSequence();
	}
}
void UARActionStateComponent::ServerStopAction_Implementation()
{
	StopAction();
}
bool UARActionStateComponent::ServerStopAction_Validate()
{
	return true;
}

void UARActionStateComponent::CooldownBegin()
{
	IsRecharing = true;
	IIARActionState* interval = InterfaceCast<IIARActionState>(GetOwner());
	if (interval)
	{
		interval->Execute_OnCooldownBegin(GetOwner());
	}
	OnCooldownBegin.Broadcast();
}
void UARActionStateComponent::CooldownEnded()
{
	IsRecharing = false;
	IIARActionState* interval = InterfaceCast<IIARActionState>(GetOwner());
	if (interval)
	{
		interval->Execute_OnCooldownEnd(GetOwner());
	}
	OnCooldownEnded.Broadcast();
}

void UARActionStateComponent::CastBegin()
{
	//IsCasting = true;
	//MulticastPlayAnimation();

	IIARActionState* interval = InterfaceCast<IIARActionState>(GetOwner());
	if (interval)
	{
		interval->Execute_OnCastStart(GetOwner());
	}
	MulticastPlayAnimation2();
	//if (GetNetMode() == ENetMode::NM_Standalone)
	//	PlayCastingAnimation();

	OnActionCastBegin.Broadcast(OwnedTags);
}
void UARActionStateComponent::CastEnd()
{
	//IsCasting = false;
	IIARActionState* interval = InterfaceCast<IIARActionState>(GetOwner());
	if (interval)
	{
		interval->Execute_OnCastEnd(GetOwner());
	}
	MulticastStopAnimation();
	//if (GetNetMode() == ENetMode::NM_Standalone)
	//	StopCastingAnimation();

	OnActionCastEnd.Broadcast();
}

void UARActionStateComponent::ActionInterval()
{
	OnActionInterval.Broadcast();
	//IsCasting = true;

	IIARActionState* interval = InterfaceCast<IIARActionState>(GetOwner());
	if (interval)
	{
		interval->Execute_OnActionInterval(GetOwner());
	}

	//if (GetNetMode() == ENetMode::NM_Standalone)
	//	PlayLoop();
	//OnRep_Casting();
	//ServerSetCastingState(true);

	MulticastPlayAnimation();
}

void UARActionStateComponent::MulticastPlayAnimation_Implementation()
{
	if (Owner)
	{
		Owner->Mesh->GetAnimInstance()->Montage_JumpToSection("CastLoop");
	}
}
void UARActionStateComponent::MulticastPlayAnimation2_Implementation()
{
	if (Owner)
	{
		Owner->PlayAnimMontage(CastingMontage);
	}
}

void UARActionStateComponent::MulticastStopAnimation_Implementation()
{
	if (Owner)
	{
		Owner->StopAnimMontage(CastingMontage);
	}
}

void UARActionStateComponent::OnRep_Casting()
{
	if (IsCasting)
	{
		PlayCastingAnimation();
		//ServerSetCastingState(false);
		//IsCasting = false;
	}
	else
	{
		StopCastingAnimation();
		//IsCasting = false;
	}
}

void UARActionStateComponent::PlayCastingAnimation()
{
	if (Owner)
	{
		Owner->PlayAnimMontage(CastingMontage);
	}
}

void UARActionStateComponent::PlayLoop()
{
	if (Owner)
	{
		Owner->Mesh->GetAnimInstance()->Montage_JumpToSection("CastLoop");
	}
}

void UARActionStateComponent::StopCastingAnimation()
{
	if (Owner)
	{
		Owner->StopAnimMontage(CastingMontage);
	}
}
void UARActionStateComponent::ServerSetCastingState_Implementation(bool State)
{
	IsCasting = State;
}
bool UARActionStateComponent::ServerSetCastingState_Validate(bool State)
{
	return true;
}