// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../Effects/AREffectPeriodic.h"
#include "../Componenets/ARAttributeComponent.h"
#include "ARTraceStatics.h"
#include "ARFXHelpersStatics.h"

UARFXHelpersStatics::UARFXHelpersStatics(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}

void UARFXHelpersStatics::SpawnTrailEffect_Implementation(UParticleSystem* trailFX, float trailSpeed, FName trailSpeedParam, FHitResult target, FName SocketName, APawn* Causer)
{
	//FVector Origin = UARTraceStatics::GetStartLocation(SocketName, Causer);
	//if (target.GetActor())
	//{
	//	if (trailFX)
	//	{
	//		UParticleSystemComponent* TrailPSC = UGameplayStatics::SpawnEmitterAtLocation(Causer, trailFX, Origin);
	//		if (TrailPSC)
	//		{
	//			const FVector AdjustedDir = (target.ImpactPoint - Origin).SafeNormal();
	//			FVector ParticleSpeed = AdjustedDir * trailSpeed;
	//			//TrailPSC->SetVectorParameter(trailSpeedParam, ParticleSpeed);
	//		}
	//	}
	//}
}

void UARFXHelpersStatics::AttachEffectToTarget(UParticleSystem* FXIn, FHitResult Target, FName AttachSocket, APawn* Causer)
{

}