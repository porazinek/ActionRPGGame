// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "Net/UnrealNetwork.h"
#include "../ARCharacter.h"
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
	bNetUseOwnerRelevancy = true;
	bReplicateInstigator = true;
	bReplicates = true;
}

void AARWeapon::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARWeapon, WeaponOwner);
}

void AARWeapon::OnRep_WeaponOwner()
{
	if (WeaponOwner)
		SetWeaponOwner(WeaponOwner);
}
void AARWeapon::SetWeaponOwner(AARCharacter* NewOwner)
{
	WeaponOwner = NewOwner;
	SetOwner(NewOwner);
}
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