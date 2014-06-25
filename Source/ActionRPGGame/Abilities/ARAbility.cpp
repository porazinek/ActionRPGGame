// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../ActionState/ARActionStateComponent.h"
#include "../Types/ARStructTypes.h"

#include "Net/UnrealNetwork.h"

#include "ARAbility.h"

AARAbility::AARAbility(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	bReplicates = true;
	SetReplicates(true);
	CastingSpeed = 1;
	bool IsCasting = false;
	CurrentCastTime = false;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;

	ActionState = PCIP.CreateDefaultSubobject<UARActionStateComponent>(this, TEXT("ActionState"));

	ActionState->SetNetAddressable();
	ActionState->SetIsReplicated(true);
}
void AARAbility::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARAbility, BlankRep);
	DOREPLIFETIME(AARAbility, CastingSpeed);
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
	if (!IsOnCooldown)
	{
		//Execute_ServerOnActionStart(this);
		//Execute_ClientOnActionStart(this);
		
		if (Role < ROLE_Authority)
		{
			ActionState->CastBegin();
		}
		
		IsBeingUsed = true;
		//PrimaryActorTick.SetTickFunctionEnable(true);
		//PrimaryActorTick.RegisterTickFunction(GetLevel());
	}
}
void AARAbility::StartAction()
{
	//PrimaryActorTick.SetTickFunctionEnable(true);
	//PrimaryActorTick.RegisterTickFunction(GetLevel());
	Execute_ServerOnActionStart(this);
	ActionState->StartAction();
}

void AARAbility::ServerStartAction_Implementation()
{
	StartAction();
}

bool AARAbility::ServerStartAction_Validate()
{
	return true;
}

//void AARPAbility::SpawnPeriodicEffect(AActor* EffectTarget, AActor* EffectCauser, float Duration, TSubclassOf<class AAREffectPeriodic> EffectType)
//{
//	if (Role < ROLE_Authority)
//	{
//		ServerSpawnPeriodicEffect(EffectTarget, EffectCauser, Duration, EffectType);
//	}
//	else
//	{
//		FPeriodicEffect PeriodicEffect;
//		if (!EffectTarget && !EffectCauser)
//			return;// PeriodicEffect;
//
//		FActorSpawnParameters SpawnInfo;
//		SpawnInfo.bNoCollisionFail = true;
//		SpawnInfo.Owner = EffectTarget;
//		//SpawnInfo.Instigator = EffectCauser;
//
//		AAREffectPeriodic* effecTemp = EffectTarget->GetWorld()->SpawnActor<AAREffectPeriodic>(EffectType, SpawnInfo);
//
//		PeriodicEffect.PeriodicEffect = effecTemp;
//		PeriodicEffect.MaxDuration = Duration;
//		PeriodicEffect.PeriodicEffect->MaxDuration = Duration;
//	}
//}
//
//void AARPAbility::ServerSpawnPeriodicEffect_Implementation(AActor* EffectTarget, AActor* EffectCauser, float Duration, TSubclassOf<class AAREffectPeriodic> EffectType)
//{
//
//}
//bool AARPAbility::ServerSpawnPeriodicEffect_Validate(AActor* EffectTarget, AActor* EffectCauser, float Duration, TSubclassOf<class AAREffectPeriodic> EffectType)
//{
//	return true;
//}