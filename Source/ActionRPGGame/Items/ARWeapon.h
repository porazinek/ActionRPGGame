// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "ARItem.h"
#include "../ActionState/IARActionState.h"
#include "../Interfaces/IARFXEffect.h"
#include "ARWeapon.generated.h"

UCLASS(minimalapi)
class AARWeapon : public AARItem, public IIARActionState, public IIARFXEffect
{
	GENERATED_UCLASS_BODY()
public:
	virtual void Tick(float DeltaSeconds) override;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	//	USkeletalMeshComponent* WeaponMesh;

	/* Tags owned by this weapon **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer OwnedTags;

	UPROPERTY(Replicated)
	class AARCharacter* WeaponOwner;

	//UFUNCTION()
	//	void OnRep_WeaponOwner();
	//void SetWeaponOwner(AARCharacter* NewOwner);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WeaponMesh)
		TSubobjectPtr<USkeletalMeshComponent> WeaponMesh;


	TSubobjectPtr<UArrowComponent> ArrowComp;
	void AttachWeapon();
	/* Weapon State (fire) - ActionState interface and component implementation **/

	/* [client] OVERIDE from IIARActionState */
	UFUNCTION()
	virtual void InputPressed() override;
	UFUNCTION()
	virtual void InputReleased() override;

	/*[Server]*/
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerStartAction();
	/*[Authorative]*/
	virtual void StartAction();

	/*[Server]*/
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerStopAction();
	/*[Authorative]*/
	virtual void StopAction();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		TSubobjectPtr<class UARActionStateComponent> WeaponState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FX Effects")
		TSubobjectPtr<class UARFXEffectComponent> FXEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FX Effects")
		TSubobjectPtr<class UARActionHitTrace> TraceHit;
};



