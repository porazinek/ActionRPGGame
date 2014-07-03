// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../Types/AREnumTypes.h"
#include "Net/UnrealNetwork.h"

#include "../BlueprintLibrary/ARTraceStatics.h"
#include "../ARCharacter.h"
#include "ParticleHelper.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

#include "Net/UnrealNetwork.h"

#include "ARFXEffectComponent.h"

UARFXEffectComponent::UARFXEffectComponent(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	SetIsReplicated(true);
}

void UARFXEffectComponent::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARFXEffectComponent, blank);
	DOREPLIFETIME(UARFXEffectComponent, IsHit);
	DOREPLIFETIME(UARFXEffectComponent, ParticleTarget);

	DOREPLIFETIME(UARFXEffectComponent, HitInfo);
	DOREPLIFETIME(UARFXEffectComponent, ImpactInfo);
}

void UARFXEffectComponent::SpawnTrailEffect_Implementation(UParticleSystem* trailFX, float trailSpeed, FName trailSpeedParam, FHitResult target, FName SocketName, APawn* Causer)
{
	FVector Origin = UARTraceStatics::GetStartLocation(SocketName, Causer);
	if (target.GetActor())
	{
		if (trailFX)
		{
			UParticleSystemComponent* TrailPSC = UGameplayStatics::SpawnEmitterAtLocation(Causer, trailFX, Origin);
			if (TrailPSC)
			{
				const FVector AdjustedDir = (target.ImpactPoint - Origin).SafeNormal();
				FVector ParticleSpeed = AdjustedDir * trailSpeed;
				//TrailPSC->SetVectorParameter(trailSpeedParam, ParticleSpeed);
			}
		}
	}
}

void UARFXEffectComponent::AttachEffectToTarget_Implementation(UParticleSystem* FXIn, FHitResult Target, FName AttachSocket, APawn* Causer)
{
	if (!Target.GetActor() && !PresitentFX)
		return;

	AARCharacter* hitTarget = Cast<AARCharacter>(Target.GetActor());
	if (!hitTarget)
		return;

	UParticleSystemComponent* AttachedPSC = UGameplayStatics::SpawnEmitterAttached(PresitentFX, hitTarget->Mesh, AttachSocket);
}

void UARFXEffectComponent::SpawnEffectOnHitLoc_Implementation(UParticleSystem* FXIn, FHitResult HitLocation, APawn* Causer)
{
	if (!HitLocation.GetActor() && !FXIn)
		return;

	UParticleSystemComponent* ImpactPSC = UGameplayStatics::SpawnEmitterAtLocation(HitLocation.GetActor(), FXIn, HitLocation.ImpactPoint);
}

void UARFXEffectComponent::OnRep_SpawnParticleEffect()
{
	if (!ParticleTarget && !PresitentFX)
		return;

	AARCharacter* hitTarget = Cast<AARCharacter>(ParticleTarget);
	if (!hitTarget)
		return;

	UParticleSystemComponent* AttachedPSC = UGameplayStatics::SpawnEmitterAttached(PresitentFX, hitTarget->GetRootComponent(), NAME_None, ImpactInfo.Location, FRotator(0,0,0), EAttachLocation::KeepWorldPosition);
}

void UARFXEffectComponent::OnRep_Hit()
{
	SimulateHitOnClients(HitInfo.Origin, HitInfo.Location, HitInfo.StartSocket);
}
void UARFXEffectComponent::SimulateHitOnClients(FVector Origin, FVector Location, FName StartSocket)
{
	//FVector Origin = UARTraceStatics::GetStartLocation(SocketName, Causer);
	//UARTraceStatics::GetHitResult(10000, StartSocket, )
	//if ()
	//{
		if (TrailFXPar)
		{
			UParticleSystemComponent* TrailPSC = UGameplayStatics::SpawnEmitterAtLocation(GetOwner(), TrailFXPar, Origin);
			if (TrailPSC)
			{
				const FVector AdjustedDir = (Location - Origin).SafeNormal();
				FVector ParticleSpeed = AdjustedDir * TrailSpeedPar;
				TrailPSC->SetVectorParameter(TrailSpeedParamName, ParticleSpeed);
			}
		}
	//}
}