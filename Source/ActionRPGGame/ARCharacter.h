// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/SpringArmComponent.h"
#include "Types/ARStructTypes.h"

#include "ARCharacter.generated.h"

UCLASS(config = Game)
class AARCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()
	
	virtual void PostInitializeComponents() OVERRIDE;
	//virtual void BeginPlay() OVERRIDE;
	virtual void PossessedBy(class AController* NewController) OVERRIDE;
	virtual void OnRep_Controller() OVERRIDE;
	//UPROPERTY()
		//TArray<FEmptyStruct> TestArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		TSubobjectPtr<class USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		TSubobjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
		TSubobjectPtr<class UARAttributeComponent> Attributes;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
		TSubobjectPtr<class UAREquipmentComponent> Equipment;

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
		TSubclassOf<class AARPAbility> AbilityToUse;

	UFUNCTION()
		void SpawnDefaultAbility();
	UPROPERTY(Replicated)
	class AARPAbility* AbilityInInventory;


	void EquipAbility(class AARPAbility* AbilityIn);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerEquipAbility(class AARPAbility* AbilityIn);

	void SetCurrentAbility(class AARPAbility* AbilityIn);


	void PickupItem();

	void SetInventoryVisibility();
	EVisibility InventoryVisibility;
	EVisibility GetInventoryVisibility();
	/*[Server] - fire after actions has been initialized*/
	UFUNCTION(BlueprintImplementableEvent)
		void OnActionInitialized();
	/*[Server]*/
	UFUNCTION(BlueprintImplementableEvent)
		void OnCharacterInitialize();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSwapLeftWeapon();

	/**[Input]*/
	void InputSwapLeftWeapon();

	void InputSwapRightWeapon();

	///////////////////////////////////////////////////////////////
	///////::: Input Handling
protected:
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	UPROPERTY(BlueprintReadOnly, Transient, ReplicatedUsing = OnRep_ActionButtonOne, Category = "Ability Bar")
		class AARPAbility* ActionButtonOne;
	UFUNCTION()
		void OnRep_ActionButtonOne();
	UFUNCTION()
		void InputActionButtonOne();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) OVERRIDE;
	// End of APawn interface

	//UPROPERTY(EditAnywhere, Category = CameraOffsets)
	//	FVector LowOffset;
	//UPROPERTY(EditAnywhere, Category = CameraOffsets)
	//	FVector MidOffset;
	//UPROPERTY(EditAnywhere, Category = CameraOffsets)
	//	FVector HighOffset;
	//virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) OVERRIDE;
};

