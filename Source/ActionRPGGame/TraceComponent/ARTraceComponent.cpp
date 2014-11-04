// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"


#include "../Componenets/AREquipmentComponent.h"
#include "../ARCharacter.h"
#include "../ARPlayerController.h"

#include "Net/UnrealNetwork.h"

#include "ARTraceComponent.h"

UARTraceComponent::UARTraceComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ITracing = Cast<IIARTracing>(GetOwner());
	if (ITracing)
	{
		ARCharOwner = ITracing->GetCharacter();
		ARPCOwner = ITracing->GetController();
	}
}
FVector UARTraceComponent::GetCameraAim()
{
	FVector FinalAim = FVector::ZeroVector;
	if (ARCharOwner)
	{
		FinalAim = ARCharOwner->GetBaseAimRotation().Vector();
	}

	return FinalAim;
}

FVector UARTraceComponent::GetCameraDamageStartLocation(const FVector& AimDir)
{
	FVector OutStartTrace = FVector::ZeroVector;
	if (ARCharOwner)
	{
		FRotator UnusedRot;
		if (ARPCOwner)
		{
			ARPCOwner->GetPlayerViewPoint(OutStartTrace, UnusedRot);
		}
		OutStartTrace = OutStartTrace + AimDir * ((ARCharOwner->GetActorLocation() - OutStartTrace) | AimDir);
	}
	return OutStartTrace;
}

USkeletalMeshComponent* UARTraceComponent::GetWeaponMesh()
{
	if (ARCharOwner)
	{
		TWeakObjectPtr<UAREquipmentComponent> eqComp = ARCharOwner->Equipment;
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

FVector UARTraceComponent::GetStartLocation(FName SocketName, TEnumAsByte<EWeaponHand> Hand)
{
	FVector Origin = FVector::ZeroVector;
	TWeakObjectPtr<USkeletalMeshComponent> OriginMesh;
	/*
	If there are no weapons, we should fallback to the sockets on hands (LeftHandSocket, RightHandSocket)
	or, Empty weapon should be considered weapons, by Equipment component.
	Either way we can't just return 0,0,0 vector.
	*/
	if (ARCharOwner)
	{
		TWeakObjectPtr<UAREquipmentComponent> eqComp = ARCharOwner->Equipment;
		if (eqComp.Get())
		{
			switch (Hand)
			{
			case EWeaponHand::WeaponLeft:
				if (eqComp->ActiveLeftHandWeapon)
				{
					OriginMesh = eqComp->ActiveLeftHandWeapon->WeaponMesh;
					return OriginMesh->GetSocketLocation(SocketName);
				}
			case EWeaponHand::WeaponRight:
				if (eqComp->ActiveRightHandWeapon)
				{
					OriginMesh = eqComp->ActiveRightHandWeapon->WeaponMesh;
					return OriginMesh->GetSocketLocation(SocketName);
				}
			default:
				return Origin;
			}
		}
	}
	return Origin;
}

FVector UARTraceComponent::GetStartLocationFromCharacter(FName SocketName, TEnumAsByte<EWeaponHand> Hand)
{
	FVector Origin = FVector::ZeroVector;
	///*
	//If there are no weapons, we should fallback to the sockets on hands (LeftHandSocket, RightHandSocket)
	//or, Empty weapon should be considered weapons, by Equipment component.
	//Either way we can't just return 0,0,0 vector.
	//*/
	//if (InitiatedBy)
	//{
	//	switch (Hand)
	//	{
	//	case EWeaponHand::WeaponLeft:
	//		if (InitiatedBy->Equipment->ActiveLeftHandWeapon)
	//		{
	//			return InitiatedBy->Equipment->ActiveLeftHandWeapon->WeaponMesh->GetSocketLocation(SocketName);
	//		}
	//	case EWeaponHand::WeaponRight:
	//		if (InitiatedBy->Equipment->ActiveRightHandWeapon)
	//		{
	//			return InitiatedBy->Equipment->ActiveRightHandWeapon->WeaponMesh->GetSocketLocation(SocketName);
	//		}
	//	default:
	//		return InitiatedBy->Mesh->GetSocketLocation(SocketName);
	//	}
	//}
	return Origin;
}
FHitResult UARTraceComponent::RangedTrace(const FVector& StartTrace, const FVector& EndTrace, TEnumAsByte<EARTraceType> TraceType)
{
	FHitResult Hit;
	return Hit;
}
FHitResult UARTraceComponent::GetHitResult(float Range, FName StartSocket, bool DrawDebug, bool UseStartSocket, TEnumAsByte<EARTraceType> TraceType, TEnumAsByte<EWeaponHand> Hand)
{
	FHitResult Impact;
	//if (!InitiatedBy)
	//	return Impact;
	//const FVector ShootDir = GetCameraAim(InitiatedBy);
	//APlayerController* con = Cast<APlayerController>(InitiatedBy->GetController());
	//if (!con)
	//	return Impact;
	//FVector WorldPosition;
	//FVector WorldDirection;
	//FVector2D s;
	//s = GEngine->GameViewport->Viewport->GetSizeXY();

	//con->DeprojectScreenPositionToWorld((s.X*0.5) - 40.0f, (s.Y*0.5) + 56.0f, WorldPosition, WorldDirection);

	////AARHUD* hud = Cast<AARHUD>(con->GetHUD());



	//FVector StartTrace = WorldPosition; // GetCameraDamageStartLocation(ShootDir, InitiatedBy);
	////const FVector EndTrace = (StartTrace + ShootDir * Range);
	//const FVector EndTrace = (WorldPosition + (WorldDirection * Range));
	//Impact = RangedTrace(StartTrace, EndTrace, InitiatedBy, TraceType);
	//if (DrawDebug)
	//{
	//	DrawDebugLine(InitiatedBy->GetWorld(), StartTrace, EndTrace, FColor::Black, true, 10.0f, 0.0f, 1.0f);
	//}
	//if (Impact.GetActor())
	//{
	//	if (UseStartSocket)
	//	{
	//		FVector Origin = GetStartLocation(StartSocket, InitiatedBy, Hand);
	//		FHitResult hitResult = RangedTrace(Origin, Impact.ImpactPoint, InitiatedBy, TraceType); //Origin + impactDir*range);

	//		if (DrawDebug)
	//		{
	//			DrawDebugLine(InitiatedBy->GetWorld(), Origin, Impact.ImpactPoint, FColor::Blue, true, 10.0f, 0.0f, 1.0f);
	//		}
	//		if (hitResult.GetActor())
	//		{
	//			DrawDebugLine(InitiatedBy->GetWorld(), Origin, Impact.ImpactPoint, FColor::Red, true, 10.0f, 0.0f, 1.0f);
	//			return hitResult;
	//		}
	//		return Impact;
	//	}
	//	//SetTargetAttributes(Impact.GetActor());
	//}
	return Impact;
}