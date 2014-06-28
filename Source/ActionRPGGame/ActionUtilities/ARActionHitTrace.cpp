// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../Types/AREnumTypes.h"
#include "Net/UnrealNetwork.h"

#include "../BlueprintLibrary/ARTraceStatics.h"
#include "../ARCharacter.h"

#include "ARActionHitTrace.h"

UARActionHitTrace::UARActionHitTrace(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}

FVector UARActionHitTrace::GetCameraAim(APawn* UsedBy)
{
	FVector FinalAim = FVector::ZeroVector;
	if (UsedBy)
	{
		FinalAim = UsedBy->GetBaseAimRotation().Vector();
	}

	return FinalAim;
}

FVector UARActionHitTrace::GetCameraDamageStartLocation(const FVector& AimDir, APawn* UsedBy)
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

USkeletalMeshComponent* UARActionHitTrace::GetWeaponMesh(APawn* InitiatedBy)
{
	if (InitiatedBy)
	{
		TWeakObjectPtr<UAREquipmentComponent> eqComp = InitiatedBy->FindComponentByClass<UAREquipmentComponent>();
		if (eqComp.Get())
		{
			if (eqComp->ActiveLeftHandWeapon)
			{
				return eqComp->ActiveLeftHandWeapon->WeaponMesh;
			}

			if (eqComp->ActiveRightHandWeapon)
			{
				return eqComp->ActiveRightHandWeapon->WeaponMesh;
			}
		}
	}
	return NULL;
}

FVector UARActionHitTrace::GetStartLocation(FName SocketName, APawn* InitiatedBy)
{
	FVector Origin = FVector::ZeroVector;
	TWeakObjectPtr<USkeletalMeshComponent> OriginMesh;

	if (InitiatedBy)
	{
		TWeakObjectPtr<UAREquipmentComponent> eqComp = InitiatedBy->FindComponentByClass<UAREquipmentComponent>();
		if (eqComp.Get())
		{
			if (eqComp->ActiveLeftHandWeapon)
			{
				OriginMesh = eqComp->ActiveLeftHandWeapon->WeaponMesh.Get();
				return OriginMesh->GetSocketLocation(SocketName);
			}

			if (eqComp->ActiveRightHandWeapon)
			{
				OriginMesh = eqComp->ActiveRightHandWeapon->WeaponMesh.Get();
				return OriginMesh->GetSocketLocation(SocketName);
			}
		}
	}
	return Origin;
}

FHitResult UARActionHitTrace::RangedTrace(const FVector& StartTrace, const FVector& EndTrace, APawn* InitiatedBy, TEnumAsByte<EARTraceType> TraceType)
{
	FHitResult Hit(ForceInit);

	static FName PowerTag = FName(TEXT("RangedTrace"));
	FCollisionQueryParams TraceParams(PowerTag, false, InitiatedBy);
	TraceParams.bTraceComplex = false;
	TraceParams.bTraceAsyncScene = false;
	TraceParams.bReturnPhysicalMaterial = true;

	switch (TraceType)
	{
	case EARTraceType::Trace_Weapon:
	{
		bool traceResult = InitiatedBy->GetWorld()->LineTraceSingle(Hit, StartTrace, EndTrace, COLLISION_WEAPON_CHANNEL, TraceParams);
		return Hit;
	}
	case EARTraceType::Trace_UI:
	{
		bool traceResult = InitiatedBy->GetWorld()->LineTraceSingle(Hit, StartTrace, EndTrace, COLLISION_GUI_CHANNEL, TraceParams);
		return Hit;
	}
	default:
		return Hit;
	}
	//bool traceResult = InitiatedBy->GetWorld()->LineTraceSingle(Hit, StartTrace, EndTrace, ECollisionChannel::ECC_WorldDynamic, TraceParams);
	return Hit;
}

FHitResult UARActionHitTrace::GetHitResult(float Range, FName StartSocket, APawn* InitiatedBy, bool DrawDebug, bool UseStartSocket, TEnumAsByte<EARTraceType> TraceType)
{
	const FVector ShootDir = GetCameraAim(InitiatedBy);

	FVector StartTrace = GetCameraDamageStartLocation(ShootDir, InitiatedBy);
	const FVector EndTrace = (StartTrace + ShootDir * Range);
	FHitResult Impact = RangedTrace(StartTrace, EndTrace, InitiatedBy, TraceType);
	if (DrawDebug)
	{
		DrawDebugLine(InitiatedBy->GetWorld(), StartTrace, EndTrace, FColor::Black, true, 10.0f, 0.0f, 1.0f);
	}
	if (Impact.GetActor())
	{
		if (UseStartSocket)
		{
			FVector Origin = GetStartLocation(StartSocket, InitiatedBy);
			FHitResult hitResult = RangedTrace(Origin, Impact.ImpactPoint, InitiatedBy, TraceType); //Origin + impactDir*range);
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