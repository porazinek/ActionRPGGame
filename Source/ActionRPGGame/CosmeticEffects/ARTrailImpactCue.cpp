// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../Types/AREnumTypes.h"

#include "../BlueprintLibrary/ARTraceStatics.h"
#include "../ARCharacter.h"
#include "../Items/ARWeapon.h"
#include "../ARCharacter.h"
#include "../Componenets/AREquipmentComponent.h"
#include "../Abilities/ARAbility.h"


#include "IARCosmeticEffects.h"

#include "ParticleHelper.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

#include "Net/UnrealNetwork.h"

#include "ARTrailImpactCue.h"

UARTrailImpactCue::UARTrailImpactCue(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicated(true);

	bWantsInitializeComponent = true;
	bAutoRegister = true;
}
void UARTrailImpactCue::InitializeComponent()
{
	Super::InitializeComponent();
}
void UARTrailImpactCue::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARTrailImpactCue, HitInfo);
}

void UARTrailImpactCue::OnRep_Hit()
{
	//HitInfo.Origin += FVector(FMath::FRandRange(-1, 1), FMath::FRandRange(-1, 1), FMath::FRandRange(-1, 1));
	SimulateHitOnClients(HitInfo.Origin, HitInfo.Location, HitInfo.StartSocket);
}
void UARTrailImpactCue::SimulateHitOnClients(FVector Origin, FVector Location, FName StartSocketIn)
{
	if (TrailFX)
	{
		IIARCosmeticEffects* cosInt = Cast<IIARCosmeticEffects>(GetOwner());
		if (cosInt)
		{
			FVector locOrigin = cosInt->GetOriginLocation();
			TrailPSC = UGameplayStatics::SpawnEmitterAtLocation(GetOwner(), TrailFX, locOrigin);
			if (TrailPSC)
			{
				const FVector AdjustedDir = (Location - locOrigin).SafeNormal();
				FVector ParticleSpeed = AdjustedDir * TrailSpeed2;
				TrailPSC->SetVectorParameter(TrailSpeedParam2, ParticleSpeed);
			}
			if (ImpactFX)
				UGameplayStatics::SpawnEmitterAtLocation(GetOwner(), ImpactFX, Location);

			return;
		}
	}
	HitInfo.HitCounter++;
}

void UARTrailImpactCue::SpawnEffectOnOwner()
{
	if (GetNetMode() == ENetMode::NM_Standalone)
		OnRep_Hit();
}
void UARTrailImpactCue::OnCollide(FName EventName, float EmitterTime, int32 ParticleTime, FVector Location, FVector Velocity, FVector Direction, FVector Normal, FName BoneName)
{
	if (TrailPSC)
	{
		TrailPSC->Deactivate();
		TrailPSC->DestroyComponent();
	}
}
//void UARTrailCue::SetOnBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//
//}