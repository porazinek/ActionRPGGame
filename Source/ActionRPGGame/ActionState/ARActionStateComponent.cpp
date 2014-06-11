// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "ARActionState.h"
#include "ARActionStateActive.h"
#include "ARACtionStateCooldown.h"
#include "ARActionStateCasting.h"

#include "Net/UnrealNetwork.h"

#include "ARActionStateComponent.h"

UARActionStateComponent::UARActionStateComponent(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	SetIsReplicated(true);
	bReplicates = true;
	bWantsInitializeComponent = true;
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

	DOREPLIFETIME(UARActionStateComponent, BlankRep);
	//DOREPLIFETIME(UARActionStateComponent, CurrentState);
}
//void UARActionStateComponent::OnRep_CurrentState()
//{
//	SetState(CurrentState);
//}
//void UARActionStateComponent::SetState(class UARActionState* StateIn)
//{
//	CurrentState = StateIn;
//}
void UARActionStateComponent::TickMe(float DeltaTime)
{
	if (CurrentState)
	{
		CurrentState->Tick(DeltaTime);
	}
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
	}
	else
	{
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

void UARActionStateComponent::CooldownBegin()
{
	OnCooldownBegin.Broadcast();
}
void UARActionStateComponent::CooldownEnded()
{
	GetOwner()->PrimaryActorTick.SetTickFunctionEnable(false);
	GetOwner()->PrimaryActorTick.UnRegisterTickFunction();
	OnCooldownEnded.Broadcast();
}

void UARActionStateComponent::CastBegin()
{
	OnActionCastBegin.Broadcast();
}
void UARActionStateComponent::CastEnd()
{
	OnActionCastEnd.Broadcast();
}

void UARActionStateComponent::ActionInterval()
{
	OnActionInterval.Broadcast();
}