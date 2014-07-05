// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../Types/AREnumTypes.h"

#include "../BlueprintLibrary/ARTraceStatics.h"
#include "../ARCharacter.h"
#include "ParticleHelper.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

#include "Net/UnrealNetwork.h"

#include "ARTrailCue.h"

UARTrailCue::UARTrailCue(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	SetIsReplicated(true);
}

void UARTrailCue::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARTrailCue, HitInfo);
}

void UARTrailCue::OnRep_Hit()
{
	SimulateHitOnClients(HitInfo.Origin, HitInfo.Location, HitInfo.StartSocket);
}
void UARTrailCue::SimulateHitOnClients(FVector Origin, FVector Location, FName StartSocket)
{
	//FVector Origin = UARTraceStatics::GetStartLocation(SocketName, Causer);
	//UARTraceStatics::GetHitResult(10000, StartSocket, )
	//if ()
	//{
		if (TrailFX)
		{
			UParticleSystemComponent* TrailPSC = UGameplayStatics::SpawnEmitterAtLocation(GetOwner(), TrailFX, Origin+100);
			if (TrailPSC)
			{
				const FVector AdjustedDir = (Location - Origin).SafeNormal();
				FVector ParticleSpeed = AdjustedDir * TrailSpeed2;
				TrailPSC->SetVectorParameter(TrailSpeedParam2, ParticleSpeed);
			}
		}
	//}
}