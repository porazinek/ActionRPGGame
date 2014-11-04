// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/SpringArmComponent.h"
#include "Types/ARStructTypes.h"
#include "SlateBasics.h"
#include "ARCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDMDOnSomethingTest, float, floatInput);

UCLASS(config = Game)
class AARCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()
public:
	UFUNCTION(NetMulticast, Reliable)
		void ClientRunDelegate();

	float testNumber;
	int32 supermegalol;

	void RunDelegate();
	UFUNCTION()
	void FunctionToCall(float input);
	FDMDOnSomethingTest OnSomethingTest;

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void PossessedBy(class AController* NewController) override;
	virtual void OnRep_Controller() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
	

	//UPROPERTY(BlueprintReadOnly, Category = "Feats")
	//	TArray<class UAREffectType*> Feats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feats")
		TArray<TSubclassOf<class UAREffectType>> FeatClasses;
	
	UPROPERTY()
		UParticleSystemComponent* PresistentParticle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
		class UARAttributeComponent* Attributes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
		class UARFeatComponent* Feats;
	/*
		Deprecated!
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
		class UAREquipmentComponent* Equipment;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
		class UARAbilityComponent* Abilities;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay, BlueprintReadOnly, Category = "Character Mesh")
		class USkeletalMeshComponent* HeadMesh;
	UPROPERTY(VisibleAnywhere, AdvancedDisplay, BlueprintReadOnly, Category = "Character Mesh")
		class USkeletalMeshComponent* ShoulderMesh;
	UPROPERTY(VisibleAnywhere, AdvancedDisplay, BlueprintReadOnly, Category = "Character Mesh")
		class USkeletalMeshComponent* ChestMesh;
	UPROPERTY(VisibleAnywhere, AdvancedDisplay, BlueprintReadOnly, Category = "Character Mesh")
		class USkeletalMeshComponent* LegsMesh;
	UPROPERTY(VisibleAnywhere, AdvancedDisplay, BlueprintReadOnly, Category = "Character Mesh")
		class USkeletalMeshComponent* HandsMesh;
	UPROPERTY(VisibleAnywhere, AdvancedDisplay, BlueprintReadOnly, Category = "Character Mesh")
		class USkeletalMeshComponent* FootMesh;

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

	void InputReloadLeftWeapon();
	void InputReloadRightWeapon();


	void InputStartCrouch();
	void InputStopCrouch();
	void InputStartJump();
	void InputStopJump();

	UPROPERTY(BlueprintReadOnly, Category = "AR Character")
		bool bIsJumpButtonPressed;
	///////////////////////////////////////////////////////////////
	///////::: Input Handling

	UPROPERTY(BlueprintReadOnly, Category = "Input")
		bool IsCharacterTurningYaw;
	UPROPERTY(BlueprintReadOnly, Category = "Input")
		float TurningDirection;
protected:
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	virtual void AddControllerYawInput(float Val);

	UPROPERTY(BlueprintReadOnly, Transient, ReplicatedUsing = OnRep_ActionButtonOne, Category = "Ability Bar")
		class AARAbility* ActionButtonOne;
	UFUNCTION()
		void OnRep_ActionButtonOne();
	UFUNCTION()
		void InputActionButtonOne();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

};

