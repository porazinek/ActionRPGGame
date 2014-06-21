// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../Effects/AREffectPeriodic.h"
#include "../Componenets/ARAttributeComponent.h"
#include "../Componenets/AREquipmentComponent.h"
#include "../Items/ARWeapon.h"
#include "../Items/ARItem.h"

#include "ARTraceStatics.h"

UARTraceStatics::UARTraceStatics(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}

FVector UARTraceStatics::GetCameraAim(APawn* UsedBy)
{
	FVector FinalAim = FVector::ZeroVector;
	if (UsedBy)
	{
		FinalAim = UsedBy->GetBaseAimRotation().Vector();
	}

	return FinalAim;
}

FVector UARTraceStatics::GetCameraDamageStartLocation(const FVector& AimDir, APawn* UsedBy)
{
	FVector OutStartTrace = FVector::ZeroVector;
	if (UsedBy)
	{
		FRotator UnusedRot;
		if (UsedBy->GetController())
		{
			UsedBy->GetController()->GetPlayerViewPoint(OutStartTrace, UnusedRot);
		}
		OutStartTrace = OutStartTrace + AimDir * ((UsedBy->GetActorLocation() - OutStartTrace) | AimDir);
	}
	return OutStartTrace;
}

USkeletalMeshComponent* UARTraceStatics::GetWeaponMesh(APawn* InitiatedBy)
{
	if (InitiatedBy)
	{
		TWeakObjectPtr<UAREquipmentComponent> eqComp = InitiatedBy->FindComponentByClass<UAREquipmentComponent>();
		if (eqComp.Get())
		{
			if (eqComp->MainHand)
			{
				return eqComp->MainHand->WeaponMesh;
			}

			if (eqComp->OffHand)
			{
				return eqComp->OffHand->WeaponMesh;
			}
		}
	}
	return NULL;
}

FVector UARTraceStatics::GetStartLocation(FName SocketName, APawn* InitiatedBy)
{
	FVector Origin = FVector::ZeroVector;
	TWeakObjectPtr<USkeletalMeshComponent> OriginMesh;

	if (InitiatedBy)
	{
		TWeakObjectPtr<UAREquipmentComponent> eqComp = InitiatedBy->FindComponentByClass<UAREquipmentComponent>();
		if (eqComp.Get())
		{
			if (eqComp->MainHand)
			{
				OriginMesh = eqComp->MainHand->WeaponMesh.Get();
				return OriginMesh->GetSocketLocation(SocketName);
			}

			if (eqComp->OffHand)
			{
				OriginMesh = eqComp->OffHand->WeaponMesh.Get();
				return OriginMesh->GetSocketLocation(SocketName);
			}
		}
	}
	return Origin;
}

FHitResult UARTraceStatics::RangedTrace(const FVector& StartTrace, const FVector& EndTrace, APawn* InitiatedBy)
{
	FHitResult Hit(ForceInit);

	static FName PowerTag = FName(TEXT("RangedTrace"));
	FCollisionQueryParams TraceParams(PowerTag, false, InitiatedBy);
	TraceParams.bTraceComplex = false;
	TraceParams.bTraceAsyncScene = false;
	TraceParams.bReturnPhysicalMaterial = true;

	bool traceResult = InitiatedBy->GetWorld()->LineTraceSingle(Hit, StartTrace, EndTrace, ECollisionChannel::ECC_WorldDynamic, TraceParams);

	return Hit;
}

FHitResult UARTraceStatics::GetHitResult(float Range, FName StartSocket, APawn* InitiatedBy, bool DrawDebug, bool UseStartSocket)
{
	const FVector ShootDir = GetCameraAim(InitiatedBy);

	FVector StartTrace = GetCameraDamageStartLocation(ShootDir, InitiatedBy);
	const FVector EndTrace = (StartTrace + ShootDir * Range);
	FHitResult Impact = RangedTrace(StartTrace, EndTrace, InitiatedBy);
	if (DrawDebug)
	{
		DrawDebugLine(InitiatedBy->GetWorld(), StartTrace, EndTrace, FColor::Black, true, 10.0f, 0.0f, 1.0f);
	}
	if (Impact.GetActor())
	{
		if (UseStartSocket)
		{
			FVector Origin = GetStartLocation(StartSocket, InitiatedBy);
			FHitResult hitResult = RangedTrace(Origin, Impact.ImpactPoint, InitiatedBy); //Origin + impactDir*range);
			if (DrawDebug)
			{
				DrawDebugLine(InitiatedBy->GetWorld(), Origin, Impact.ImpactPoint, FColor::Blue, true, 10.0f, 0.0f, 1.0f);
			}
			if (hitResult.GetActor())
			{
				DrawDebugLine(InitiatedBy->GetWorld(), Origin, Impact.ImpactPoint, FColor::Red, true, 10.0f, 0.0f, 1.0f);
				return hitResult;
			}
			return Impact;
		}
		//SetTargetAttributes(Impact.GetActor());
	}
	return Impact;
}