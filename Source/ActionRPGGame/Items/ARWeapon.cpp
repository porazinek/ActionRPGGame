// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "Net/UnrealNetwork.h"
#include "../ARCharacter.h"
#include "../ActionState/ARActionStateComponent.h"
#include "../FXEffects/ARFXEffectComponent.h"

#include "ParticleHelper.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

#include "ARWeapon.h"

AARWeapon::AARWeapon(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	ArrowComp = PCIP.CreateDefaultSubobject<UArrowComponent>(this, TEXT("ArrowComp"));
	WeaponMesh = PCIP.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("WeaponMesh"));
	WeaponMesh->AlwaysLoadOnClient = true;
	WeaponMesh->AlwaysLoadOnServer = true;
	WeaponMesh->AttachParent = ArrowComp;
	//WeaponMesh->SetNetAddressable();
	//WeaponMesh->SetIsReplicated(true);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;

	WeaponState = PCIP.CreateDefaultSubobject<UARActionStateComponent>(this, TEXT("ActionState"));
	WeaponState->Owner = WeaponOwner;
	WeaponState->SetNetAddressable();
	WeaponState->SetIsReplicated(true);

	FXEffect = PCIP.CreateDefaultSubobject<UARFXEffectComponent>(this, TEXT("FXEffects"));
	FXEffect->SetIsReplicated(true);
	FXEffect->SetNetAddressable();
	TraceHit = PCIP.CreateDefaultSubobject<UARActionHitTrace>(this, TEXT("TraceHit"));

	bNetUseOwnerRelevancy = true;
	bReplicateInstigator = true;
	bReplicates = true;
}
void AARWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	WeaponState->TickMe(DeltaSeconds);

}

void AARWeapon::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARWeapon, WeaponOwner);
	DOREPLIFETIME(AARWeapon, HitInfo);
}

//void AARWeapon::OnRep_WeaponOwner()
//{
//	if (WeaponOwner)
//		SetWeaponOwner(WeaponOwner);
//}
//void AARWeapon::SetWeaponOwner(AARCharacter* NewOwner)
//{
//	WeaponOwner = NewOwner;
//	SetOwner(NewOwner);
//}
void AARWeapon::AttachWeapon()
{
	if (WeaponOwner)
	{
		// Remove and hide both first and third person meshes

		// For locally controller players we attach both weapons and let the bOnlyOwnerSee, bOwnerNoSee flags deal with visibility.
		//FName AttachPoint = WeaponOwner->GetWeaponAttachPoint();
		if (WeaponOwner->IsLocallyControlled() == true)
		{
			USkeletalMeshComponent* PawnMesh3p = WeaponOwner->Mesh;
			WeaponMesh->SetHiddenInGame(false);
			WeaponMesh->AttachTo(PawnMesh3p, "TestSocket");
		}
		else
		{
			USkeletalMeshComponent* UseWeaponMesh = WeaponMesh;
			USkeletalMeshComponent* UsePawnMesh = WeaponOwner->Mesh;
			UseWeaponMesh->AttachTo(UsePawnMesh, "TestSocket");
			UseWeaponMesh->SetHiddenInGame(false);
		}
	}
}

void AARWeapon::InputPressed()
{

	//PrimaryActorTick.bStartWithTickEnabled = true;
	if (Role < ROLE_Authority)
	{
		Execute_ClientOnActionStart(this);
		//WeaponState->StartAction();
		ServerStartAction();
	}
	else
	{
		StartAction(); //we are on server, we just call normal version.
	}
}
//does do anything for now, but it will be needed for fireting stop ;).
void AARWeapon::InputReleased()
{
	if (Role < ROLE_Authority)
	{
		//WeaponState->StopAction();
		ServerStopAction();
	}
	else
	{
		StopAction();
	}
}
void AARWeapon::StartAction()
{
	Execute_ServerOnActionStart(this);
	WeaponState->StartAction();
}
void AARWeapon::ServerStartAction_Implementation()
{
	StartAction();
}
bool AARWeapon::ServerStartAction_Validate()
{
	return true;
}

void AARWeapon::StopAction()
{
	Execute_ServerOnActionStop(this);
	WeaponState->StopAction();
}
void AARWeapon::ServerStopAction_Implementation()
{
	StopAction();
}
bool AARWeapon::ServerStopAction_Validate()
{
	return true;
}


void AARWeapon::OnRep_HitInfo()
 {
	SimulateHitOnClients(HitInfo.Origin, HitInfo.Location, HitInfo.StartSocket);
}
void AARWeapon::SimulateHitOnClients(FVector Origin, FVector Location, FName StartSocket)
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