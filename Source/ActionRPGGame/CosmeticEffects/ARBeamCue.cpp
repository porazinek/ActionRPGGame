// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../Types/AREnumTypes.h"

#include "../BlueprintLibrary/ARTraceStatics.h"
#include "../ARCharacter.h"

#include "../Items/ARWeapon.h"

#include "ParticleHelper.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

#include "Net/UnrealNetwork.h"

#include "ARBeamCue.h"

UARBeamCue::UARBeamCue(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	SetIsReplicated(true);

	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bRunOnAnyThread = true;
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_DuringPhysics;
	bTickInEditor = true;
	bAllowConcurrentTick = true;
	bAutoRegister = true;
	bWantsInitializeComponent = true;
	IsFiring = false;
	UpdateInterval = 0.1f;
}

void UARBeamCue::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARBeamCue, HitInfo);
	DOREPLIFETIME(UARBeamCue, IsFiring);
}

void UARBeamCue::InitializeComponent()
{
	Super::InitializeComponent();

	Activate();
}

void UARBeamCue::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsFiring)
	{
		CurrentUpdateTime += DeltaTime;
		if (CurrentUpdateTime >= UpdateInterval)
		{
			CurrentUpdateTime = 0;

			AARWeapon* weap = Cast<AARWeapon>(GetOwner());
			if (weap)
			{
				FHitResult hit = UARTraceStatics::GetHitResult(10000, NAME_None, weap->WeaponOwner, false, false, EARTraceType::Trace_Weapon, EWeaponHand::WeaponRight);
				HitInfo.Location = hit.Location;
			}
		}
		PlayEffectInSinglePlayer();
	}
	if (!IsFiring)
	{
		if (BeamPSC.IsValid())
			BeamPSC->DeactivateSystem();
	}
}

void UARBeamCue::DestroyComponent()
{
	Super::DestroyComponent();
}

void UARBeamCue::OnRep_Hit()
{
	SimulateHitOnClients(HitInfo.Origin, HitInfo.Location, HitInfo.StartSocket);
}
void UARBeamCue::SimulateHitOnClients(FVector Origin, FVector Location, FName StartSocket)
{
		if (TrailFX)
		{
			AARWeapon* weap = Cast<AARWeapon>(GetOwner());
			if (weap)
			{
				FVector locOrigin = UARTraceStatics::GetStartLocation(StartSocket, weap->WeaponOwner, weap->WeaponHand);
				if (!BeamPSC.IsValid())
					BeamPSC = UGameplayStatics::SpawnEmitterAtLocation(GetOwner(), TrailFX, locOrigin);
				if (BeamPSC.IsValid())
				{
					BeamPSC->SetVectorParameter(OriginParam, locOrigin);
					BeamPSC->SetVectorParameter(ImpactParam, Location);
				}
				return;
			}

		}
}

void UARBeamCue::PlayEffectInSinglePlayer()
{
	if (GetNetMode() == ENetMode::NM_Standalone)
		OnRep_Hit();
}