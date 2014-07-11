// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"
#include "../Componenets/ARAttributeBaseComponent.h"
#include "../ARCharacter.h"
#include "Net/UnrealNetwork.h"

#include "ParticleHelper.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

#include "AREffectPeriodic.h"

AAREffectPeriodic::AAREffectPeriodic(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	PeriodDuration = 1;
	CurrentDuration = 0;
	IsEffectActive = false;
}

void AAREffectPeriodic::TickMe(float DeltaTime)
{
	if (IsEffectActive)
	{
		CurrentDuration += DeltaTime;
		CurrentPeriodDuration += DeltaTime;
		if (CurrentPeriodDuration >= PeriodDuration)
		{
			CurrentPeriodDuration = 0;
			OnEffectInterval.Broadcast();
			OnEffectPeriod();
		}
		if (CurrentDuration >= MaxDuration)
		{
			CurrentDuration = 0;
			IsEffectActive = false;
			OnEffectRemoved.Broadcast();
			Deactivate();
		}
	}
}

void AAREffectPeriodic::PostNetReceive()
{
	Super::PostNetReceive();
}

void AAREffectPeriodic::Initialze()
{
	if (Role < ROLE_Authority)
	{
		ServerActivate();
	}
	else
	{
		Activate();
	}
}

void AAREffectPeriodic::Activate()
{
	OnEffectInitialized();
	OnEffectActivated.Broadcast();
	IsEffectActive = true;
}

void AAREffectPeriodic::ServerActivate_Implementation()
{
	Activate();
}

bool AAREffectPeriodic::ServerActivate_Validate()
{
	return true;
}

void AAREffectPeriodic::Deactivate()
{
	if (!EffectTarget)
		return;
	OnEffectEnd();
	UARAttributeBaseComponent* AttrComp = EffectTarget->FindComponentByClass<UARAttributeBaseComponent>();

	if (!AttrComp)
		return;
	if (IsEffectActive)
		return;
	AttrComp->RemovePeriodicEffect(this);

	CurrentDuration = 0;

	
}
void AAREffectPeriodic::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAREffectPeriodic, IsEffectActive);
	DOREPLIFETIME(AAREffectPeriodic, EffectTarget);
	//DOREPLIFETIME(AAREffectPeriodic, EffectCauser);
}

void AAREffectPeriodic::OnRep_EffectActive()
{
	if (!EffectTarget)
		return;

	AARCharacter* MyChar = Cast<AARCharacter>(EffectTarget);
	if (!MyChar)
		return;

	if (IsEffectActive)
	{

		for (FEffectCue& cue : EffectCues)
		{

		}
		if (PresitentFX)
		{
			MyChar->PresistentParticle = UGameplayStatics::SpawnEmitterAttached(PresitentFX, MyChar->Mesh, AttachLocation, FVector(0,0,0), FRotator(0, 0, 0), EAttachLocation::Type::SnapToTarget);
		}
	}
	else
	{
		//MyChar->PresistentParticle->Deactivate();
	}
}