// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "ARItem.h"
#include "../ActionState/IARActionState.h"
#include "../Types/ARAttributeTypes.h"
#include "../Types/AREnumTypes.h"



#include "ARWeapon.generated.h"

/*
	Possible refactor.

	Instead of separate weapon class, treat wepons as abilities. And differientate them by simple enum
	or Tag.

	Another possibility is to leave weapon, but add Ability subobject, which will be used to execute
	weapon actions.
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDMDOnWeaponInitialized);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDMDOnWeaponFired, TArray<FAttribute>, OutgoingDamage);

UCLASS(minimalapi)
class AARWeapon : public AARItem, public IIARActionState
{
	GENERATED_UCLASS_BODY()
public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void Destroy();

	virtual void BeginPlay() override;

	virtual void Initialize();

	/*
		Counter increased in functions which should trigger OnRep_, regardless of their state.
		It won't be used but I needed it to make this class compile!
	*/
	UPROPERTY(Replicated)
		int8 ReplicationHelper;

	UPROPERTY(EditAnywhere, Category = "Tags")
		FGameplayTagContainer OwnedTags;

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
		TEnumAsByte<EWeaponHand> WeaponHand;


	/*
		Cosmetic Properties
	*/
	/*
		Weapon Mesh
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cosmetics")
		USkeletalMeshComponent* WeaponMesh;
	/*
		Slot to which weapon will try to attach itself when sheathed (not active but, equiped).
	*/
	UPROPERTY(EditAnywhere, Category = "Cosmetics")
		TEnumAsByte<EAttachmentType> AttachmentType;
	/*
		Socket to which weapon was attached before becoming active.
		So after switching weapon, we can attach it the same place again.
	*/
	UPROPERTY()
		FName LastAttachmentSocket;

	/*
		Montage to play when this weapon is equiped and character is moving.
		You will want chracter top body part to behave differently depending on which weapon is equiped.
		You obviously don't want character with Staff to hold it like it is Gun ;).
	*/
	UPROPERTY(EditAnywhere, Category = "Cosmetics")
		UAnimMontage* MovementMontage;


	UArrowComponent* ArrowComp;
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
		class UARActionStateComponent* WeaponState;
};



