// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "ARItem.h"
#include "ARWeapon.generated.h"

UCLASS(minimalapi)
class AARWeapon : public AARItem
{
	GENERATED_UCLASS_BODY()

		//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
		//	USkeletalMeshComponent* WeaponMesh;

		UPROPERTY(ReplicatedUsing=OnRep_WeaponOwner)
		class AARCharacter* WeaponOwner;
		UFUNCTION()
		void OnRep_WeaponOwner();
		void SetWeaponOwner(AARCharacter* NewOwner);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WeaponMesh)
	TSubobjectPtr<USkeletalMeshComponent> WeaponMesh;

	TSubobjectPtr<UArrowComponent> ArrowComp;

	void AttachWeapon();

};



