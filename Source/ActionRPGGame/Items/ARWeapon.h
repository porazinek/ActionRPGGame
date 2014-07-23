// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "ARItem.h"
#include "../ActionState/IARActionState.h"
#include "../Interfaces/IARFXEffect.h"
#include "../Types/ARAttributeTypes.h"

#include "ARWeapon.generated.h"

/*
	Possible refactor.

	Instead of separate weapon class, treat wepons as abilities. And differientate them by simple enum
	or Tag.

	Another possibility is to leave weapon, but add Ability subobject, which will be used to execute
	weapon actions.
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDMDOnWeaponInitialized);

UCLASS(minimalapi)
class AARWeapon : public AARItem, public IIARActionState, public IIARFXEffect
{
	GENERATED_UCLASS_BODY()
public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void Destroy();

	virtual void Initialize();
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Owner")
	class AARCharacter* WeaponOwner;

	UPROPERTY(BlueprintReadOnly, Category = "Owner")
	class AARPlayerController* OwningController;

	UPROPERTY(EditAnywhere, Category = "Tags")
		FGameplayTagContainer OwnedTags;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WeaponMesh)
		TSubobjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		TArray<FAttribute> Attributes;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		float Damage;

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

	UFUNCTION(BlueprintImplementableEvent)
		void OnWeaponActive();
	
	UFUNCTION(BlueprintImplementableEvent)
		void OnWeaponDeactive();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		TSubobjectPtr<class UARActionStateComponent> WeaponState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FX Effects")
		TSubobjectPtr<class UARFXEffectComponent> FXEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FX Effects")
		TSubobjectPtr<class UARActionHitTrace> TraceHit;
};



