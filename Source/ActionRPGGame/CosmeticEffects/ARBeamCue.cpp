// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../Types/AREnumTypes.h"

#include "../BlueprintLibrary/ARTraceStatics.h"
#include "../ARCharacter.h"
#include "ParticleHelper.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

#include "Net/UnrealNetwork.h"

#include "ARBeamCue.h"

UARBeamCue::UARBeamCue(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	SetIsReplicated(true);
}

void UARBeamCue::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARBeamCue, HitInfo);
}

void UARBeamCue::OnRep_Hit()
{
	SimulateHitOnClients(HitInfo.Origin, HitInfo.Location, HitInfo.StartSocket);
}
void UARBeamCue::SimulateHitOnClients(FVector Origin, FVector Location, FName StartSocket)
{
	//FVector HitPoint = UARTraceStatics::RangedTrace(Origin, Location,)
	//UARTraceStatics::GetHitResult(10000, StartSocket, )
	//if ()
	//{
		if (TrailFX)
		{
			UParticleSystemComponent* TrailPSC = UGameplayStatics::SpawnEmitterAtLocation(GetOwner(), TrailFX, Origin);
			if (TrailPSC)
			{
				TrailPSC->SetVectorParameter(OriginParam, Origin);
				TrailPSC->SetVectorParameter(ImpactParam, Location);
			}
		}
	//}
}