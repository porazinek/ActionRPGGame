// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../Effects/AREffectPeriodic.h"
#include "../Componenets/ARAttributeComponent.h"
#include "../Componenets/AREquipmentComponent.h"
#include "../Items/ARWeapon.h"
#include "../Items/ARItem.h"

#include "../ARCharacter.h"

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

FVector UARTraceStatics::GetStartLocation(FName SocketName, APawn* InitiatedBy, TEnumAsByte<EWeaponHand> Hand)
{
	FVector Origin = FVector::ZeroVector;
	TWeakObjectPtr<USkeletalMeshComponent> OriginMesh;
	/*
		If there are no weapons, we should fallback to the sockets on hands (LeftHandSocket, RightHandSocket)
		or, Empty weapon should be considered weapons, by Equipment component.
		Either way we can't just return 0,0,0 vector.
		*/
	if (InitiatedBy)
	{
		TWeakObjectPtr<UAREquipmentComponent> eqComp = InitiatedBy->FindComponentByClass<UAREquipmentComponent>();
		if (eqComp.Get())
		{
			switch (Hand)
			{
			case EWeaponHand::WeaponLeft:
				if (eqComp->ActiveLeftHandWeapon)
				{
					OriginMesh = eqComp->ActiveLeftHandWeapon->WeaponMesh.Get();
					return OriginMesh->GetSocketLocation(SocketName);
				}
			case EWeaponHand::WeaponRight:
				if (eqComp->ActiveRightHandWeapon)
				{
					OriginMesh = eqComp->ActiveRightHandWeapon->WeaponMesh.Get();
					return OriginMesh->GetSocketLocation(SocketName);
				}
			default:
				return Origin;
			}
		}
	}
	return Origin;
}

FVector UARTraceStatics::GetStartLocationFromCharacter(FName SocketName, class AARCharacter* InitiatedBy, TEnumAsByte<EWeaponHand> Hand)
{
	FVector Origin = FVector::ZeroVector;
	/*
	If there are no weapons, we should fallback to the sockets on hands (LeftHandSocket, RightHandSocket)
	or, Empty weapon should be considered weapons, by Equipment component.
	Either way we can't just return 0,0,0 vector.
	*/
	if (InitiatedBy)
	{
		switch (Hand)
		{
		case EWeaponHand::WeaponLeft:
			if (InitiatedBy->Equipment->ActiveLeftHandWeapon)
			{
				return InitiatedBy->Equipment->ActiveLeftHandWeapon->WeaponMesh->GetSocketLocation(SocketName);
			}
		case EWeaponHand::WeaponRight:
			if (InitiatedBy->Equipment->ActiveRightHandWeapon)
			{
				return InitiatedBy->Equipment->ActiveRightHandWeapon->WeaponMesh->GetSocketLocation(SocketName);
			}
		default:
			return InitiatedBy->Mesh->GetSocketLocation(SocketName);
		}
	}
	return Origin;
}

FHitResult UARTraceStatics::RangedTrace(const FVector& StartTrace, const FVector& EndTrace, APawn* InitiatedBy, TEnumAsByte<EARTraceType> TraceType)
{
	FHitResult Hit(ForceInit);
	if (!InitiatedBy)
		return Hit;


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

/*
	Add some small hit location randomization (for replication), or
	*/
FHitResult UARTraceStatics::GetHitResult(float Range, FName StartSocket, APawn* InitiatedBy, bool DrawDebug, bool UseStartSocket, TEnumAsByte<EARTraceType> TraceType, TEnumAsByte<EWeaponHand> Hand)
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
			FVector Origin = GetStartLocation(StartSocket, InitiatedBy, Hand);
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

FHitResult UARTraceStatics::GetHitResultCorrected(float Range, FName StartSocket, AARCharacter* InitiatedBy, bool DrawDebug, bool UseStartSocket, TEnumAsByte<EARTraceType> TraceType, TEnumAsByte<EWeaponHand> Hand, FName CorrectionSocket)
{
	const FVector ShootDir = GetCameraAim(InitiatedBy);

	//FVector StartTrace = GetCameraDamageStartLocation(ShootDir, InitiatedBy);
	FVector StartTrace = InitiatedBy->Mesh->GetSocketLocation(CorrectionSocket);

	//const FVector EndTrace = (StartTrace + ShootDir * Range);
	const FVector EndTrace = (GetCameraDamageStartLocation(ShootDir, InitiatedBy) + ShootDir * Range);

	FHitResult Impact = RangedTrace(StartTrace, EndTrace, InitiatedBy, TraceType);
	if (DrawDebug)
	{
		DrawDebugLine(InitiatedBy->GetWorld(), StartTrace, EndTrace, FColor::Black, true, 10.0f, 0.0f, 1.0f);
	}
	if (Impact.GetActor())
	{
		if (UseStartSocket)
		{
			FVector Origin = GetStartLocation(StartSocket, InitiatedBy, Hand);
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