// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"
#include "../BlueprintLibrary/ARTraceStatics.h"
#include "IARFXEffect.h"

UIARFXEffect::UIARFXEffect(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{

}

//void IIARFXEffect::SpawnTrailEffect_Implementation(UParticleSystem* trailFX, float trailSpeed, FName trailSpeedParam, FHitResult target, FName SocketName, APawn* Causer)
//{
//	FVector Origin = UARTraceStatics::GetStartLocation(SocketName, Causer);
//	if (target.GetActor())
//	{
//		if (trailFX)
//		{
//			UParticleSystemComponent* TrailPSC = UGameplayStatics::SpawnEmitterAtLocation(Causer, trailFX, Origin);
//			if (TrailPSC)
//			{
//				const FVector AdjustedDir = (target.ImpactPoint - Origin).SafeNormal();
//				FVector ParticleSpeed = AdjustedDir * trailSpeed;
//				TrailPSC->SetVectorParameter(trailSpeedParam, ParticleSpeed);
//			}
//		}
//	}
//}