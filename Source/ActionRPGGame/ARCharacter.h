// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/SpringArmComponent.h"
#include "ARCharacter.generated.h"

UCLASS(config = Game)
class AARCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

	virtual void PostInitializeComponents() OVERRIDE;

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

	UPROPERTY(EditAnywhere, Category = CameraOffsets)
		FVector LowOffset;
	UPROPERTY(EditAnywhere, Category = CameraOffsets)
		FVector MidOffset;
	UPROPERTY(EditAnywhere, Category = CameraOffsets)
		FVector HighOffset;
	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) OVERRIDE;
};

