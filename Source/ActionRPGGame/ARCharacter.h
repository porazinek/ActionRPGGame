// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/SpringArmComponent.h"
#include "Types/ARStructTypes.h"
#include "Slate.h"
#include "ARCharacter.generated.h"

UCLASS(config = Game)
class AARCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()
	
		virtual void PostInitializeComponents() override;
	//virtual void BeginPlay() OVERRIDE;
	virtual void PossessedBy(class AController* NewController) override;
	virtual void OnRep_Controller() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		TSubobjectPtr<class USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		TSubobjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
		TSubobjectPtr<class UARAttributeComponent> Attributes;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
		TSubobjectPtr<class UAREquipmentComponent> Equipment;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
		TSubobjectPtr<class UARAbilityComponent> Abilities;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay, BlueprintReadOnly, Category = "Character Mesh")
		TSubobjectPtr<class USkeletalMeshComponent> HeadMesh;
	UPROPERTY(VisibleAnywhere, AdvancedDisplay, BlueprintReadOnly, Category = "Character Mesh")
		TSubobjectPtr<class USkeletalMeshComponent> ShoulderMesh;
	UPROPERTY(VisibleAnywhere, AdvancedDisplay, BlueprintReadOnly, Category = "Character Mesh")
		TSubobjectPtr<class USkeletalMeshComponent> ChestMesh;
	UPROPERTY(VisibleAnywhere, AdvancedDisplay, BlueprintReadOnly, Category = "Character Mesh")
		TSubobjectPtr<class USkeletalMeshComponent> LegsMesh;
	UPROPERTY(VisibleAnywhere, AdvancedDisplay, BlueprintReadOnly, Category = "Character Mesh")
		TSubobjectPtr<class USkeletalMeshComponent> HandsMesh;
	UPROPERTY(VisibleAnywhere, AdvancedDisplay, BlueprintReadOnly, Category = "Character Mesh")
		TSubobjectPtr<class USkeletalMeshComponent> FootMesh;

	/////TESTING ONLY
	UPROPERTY(EditAnywhere, Category = "Abilities")
		TSubclassOf<class AARAbility> AbilityToUse;

	UFUNCTION()
		void SpawnDefaultAbility();
	UPROPERTY(Replicated)
	class AARAbility* AbilityInInventory;


	void EquipAbility(class AARAbility* AbilityIn);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerEquipAbility(class AARAbility* AbilityIn);

	void SetCurrentAbility(class AARAbility* AbilityIn);

	/*[Server] - fire after actions has been initialized*/
	UFUNCTION(BlueprintImplementableEvent)
		void OnActionInitialized();
	/*[Server]*/
	UFUNCTION(BlueprintImplementableEvent)
		void OnCharacterInitialize();

	/**[Input]*/
	void InputSwapLeftWeapon();

	void InputSwapRightWeapon();

	//ok, it's not really an input per se, as it is not bound in character class input
	//but it going trough input from PlayerController.
	void InputFireLeftWeapon();
	void InputFireRightWeapon();
	void InputStopFireLeftWeapon();
	void InputStopFireRightWeapon();
	///////////////////////////////////////////////////////////////
	///////::: Input Handling
protected:
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	UPROPERTY(BlueprintReadOnly, Transient, ReplicatedUsing = OnRep_ActionButtonOne, Category = "Ability Bar")
		class AARAbility* ActionButtonOne;
	UFUNCTION()
		void OnRep_ActionButtonOne();
	UFUNCTION()
		void InputActionButtonOne();

	void PickupItem();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

};

