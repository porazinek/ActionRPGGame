// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "Abilities/ARAbility.h"
#include "ActionState/IARActionState.h"
#include "ActionState/ARActionStateComponent.h"
#include "Componenets/ARAttributeComponent.h"
#include "Componenets/AREquipmentComponent.h"
#include "Componenets/ARAbilityComponent.h"

#include "Types/ARAttributeTypes.h"
#include "Effects/AREffectType.h"
#include "Items/ARWeapon.h"
#include "ARPlayerController.h"

//#include "Slate.h"

#include "Net/UnrealNetwork.h"

#include "ARCharacter.h"

//////////////////////////////////////////////////////////////////////////
// AActionRPGGameCharacter

AARCharacter::AARCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// Set size for collision capsule
	CapsuleComponent->InitCapsuleSize(42.f, 96.0f);
	IsCharacterTurningYaw = false;
	//LowOffset = FVector(-380.0f, 35.0f, 15.0f);
	//MidOffset = FVector(-380.0f, 35.0f, 60.0f);
	//HighOffset = FVector(-380.0f, 35.0f, 150.0f); //x,y,z

	CameraBoom = PCIP.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->AttachTo(CapsuleComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character
	CameraBoom->bUseControllerViewRotation = true; // Rotate the arm based on the controller
	CameraBoom->SocketOffset = FVector(0.0f, 50.0f, 100.0f);
	// Create a follow camera
	FollowCamera = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUseControllerViewRotation = false; // Camera does not rotate relative to arm

	bReplicates = true;
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	CharacterMovement->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	CharacterMovement->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	CharacterMovement->JumpZVelocity = 600.f;
	CharacterMovement->AirControl = 0.2f;

	Attributes = PCIP.CreateDefaultSubobject<UARAttributeComponent>(this, TEXT("Attributes"));
	Attributes->Activate();
	Attributes->bAutoRegister = true;
	Attributes->SetIsReplicated(true);

	Equipment = PCIP.CreateDefaultSubobject<UAREquipmentComponent>(this, TEXT("Equipment"));
	Equipment->Activate();
	Equipment->bAutoRegister = true;
	//Equipment->GetNetAddressable();
	Equipment->SetIsReplicated(true);

	Abilities = PCIP.CreateDefaultSubobject<UARAbilityComponent>(this, TEXT("Abilities"));
	Abilities->Activate();
	Abilities->bAutoRegister = true;
	//Equipment->GetNetAddressable();
	Abilities->SetIsReplicated(true);

	HeadMesh = PCIP.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("HeadMesh"));
	HeadMesh->AttachParent = Mesh;
	HeadMesh->SetMasterPoseComponent(Mesh);

	ShoulderMesh = PCIP.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("ShoulderMesh"));
	ShoulderMesh->AttachParent = Mesh;
	ShoulderMesh->SetMasterPoseComponent(Mesh);

	ChestMesh = PCIP.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("ChestMesh"));
	ChestMesh->AttachParent = Mesh;
	ChestMesh->SetMasterPoseComponent(Mesh);

	LegsMesh = PCIP.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("LegsMesh"));
	LegsMesh->AttachParent = Mesh;
	LegsMesh->SetMasterPoseComponent(Mesh);

	HandsMesh = PCIP.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("HandsMesh"));
	HandsMesh->AttachParent = Mesh;
	HandsMesh->SetMasterPoseComponent(Mesh);

	FootMesh = PCIP.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("FootMesh"));
	FootMesh->AttachParent = Mesh;
	FootMesh->SetMasterPoseComponent(Mesh);
	//SetRootComponent(Mesh);
}

void AARCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (Role == ROLE_Authority)
	{
		OnCharacterInitialize();
		SpawnDefaultAbility();

		for (TSubclassOf<UAREffectType> featClass : FeatClasses)
		{
			UAREffectType* effect = ConstructObject<UAREffectType>(featClass);
			if (effect)
			{
				effect->EffectTarget = this;
				effect->EffectCausedBy = this;
				effect->EffectInstigator = this;
				//effect->AddToRoot();
				effect->Initialize();
				Feats.Add(effect);
			}
		}
	}
}
void AARCharacter::SpawnDefaultAbility()
{
	if (Role < ROLE_Authority)
		return;

	if (AbilityToUse)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.bNoCollisionFail = true;
		SpawnInfo.Owner = this;
		SpawnInfo.Instigator = this;
		AARAbility* abilityTemp = GetWorld()->SpawnActor<AARAbility>(AbilityToUse, SpawnInfo);
		AbilityInInventory = abilityTemp;
		if (AbilityInInventory)
		{
			EquipAbility(AbilityInInventory);
		}
	}
	OnActionInitialized();
}
void AARCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();
	Equipment->TargetCharacter = this;
	Equipment->TargetController = Cast<AARPlayerController>(GetController());

	Abilities->OwningCharacter = this;
	Abilities->OwningController = Cast<AARPlayerController>(GetController());
}
void AARCharacter::PossessedBy(class AController* NewController)
{
	Super::PossessedBy(NewController);
	Equipment->TargetCharacter = this;
	Equipment->TargetController = Cast<AARPlayerController>(GetController());

	Abilities->OwningCharacter = this;
	Abilities->OwningController = Cast<AARPlayerController>(GetController());
}


void AARCharacter::EquipAbility(class AARAbility* AbilityIn)
{
	if (AbilityIn)
	{
		if (Role == ROLE_Authority)
		{
			SetCurrentAbility(AbilityIn);
		}
		else
		{
			ServerEquipAbility(AbilityIn);
		}
	}
}

void AARCharacter::ServerEquipAbility_Implementation(class AARAbility* AbilityIn)
{
	EquipAbility(AbilityIn);
}

bool AARCharacter::ServerEquipAbility_Validate(class AARAbility* AbilityIn)
{
	return true;
}

void AARCharacter::SetCurrentAbility(class AARAbility* AbilityIn)
{
	ActionButtonOne = AbilityIn;
}

void AARCharacter::OnRep_ActionButtonOne()
{
	SetCurrentAbility(ActionButtonOne);
}

void AARCharacter::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AARCharacter, AbilityInInventory);
	//DOREPLIFETIME_CONDITION(AARCharacter, ActionButtonOne, COND_OwnerOnly);
	DOREPLIFETIME(AARCharacter, ActionButtonOne);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AARCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	InputComponent->BindAxis("MoveForward", this, &AARCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AARCharacter::MoveRight);

	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	//Action Bar
	//InputComponent->BindAction("ActionButtonOne", IE_Pressed, this, &AARCharacter::InputActionButtonOne);

	InputComponent->BindAction("PickupItem", IE_Pressed, this, &AARCharacter::PickupItem);

	//Add weapon switch for left and right hand.
	//InputComponent->BindAction("SwapLeftWeapon", IE_Pressed, this, &AARCharacter::InputSwapLeftWeapon);
	//InputComponent->BindAction("SwapRightWeapon", IE_Pressed, this, &AARCharacter::InputSwapRightWeapon);
}

void AARCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		//if (Value < 0)
		//{
		//	float orignal = CharacterMovement->MaxWalkSpeed;
		//	CharacterMovement->MaxWalkSpeed = CharacterMovement->MaxWalkSpeed / 2;
		//	FRotator Rotation = GetBaseAimRotation();

		//	FVector Location; //not used, just need for below.
		//	//Controller->GetPlayerViewPoint(Location, Rotation);
		//	Rotation.Normalize();
		//	FRotator YawRotation(0, Rotation.Yaw, 0);
		//	//const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis( EAxis::X );
		//	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		//	AddMovementInput(Direction, Value);
		//	CharacterMovement->MaxWalkSpeed = orignal;
		//	return;
		//}
		FRotator Rotation = GetBaseAimRotation();

		FVector Location; //not used, just need for below.
		//Controller->GetPlayerViewPoint(Location, Rotation);
		Rotation.Normalize();
		FRotator YawRotation(0, Rotation.Yaw, 0);
		//const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis( EAxis::X );
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AARCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		//const FRotator Rotation = Controller->GetPlayerViewPoint();
		FRotator Rotation;
		FVector Location; //not used, just need for below.
		Controller->GetPlayerViewPoint(Location, Rotation);
		Rotation.Normalize();
		FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		//const FVector Direction = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::Y);
		const FVector Direction = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AARCharacter::AddControllerYawInput(float Val)
{
	Super::AddControllerYawInput(Val);
	if (Val != 0)
		IsCharacterTurningYaw = true;
	else
		IsCharacterTurningYaw = false;
}
void AARCharacter::InputActionButtonOne()
{
	if (Abilities->ActionBarOne[1].Ability.IsValid())
	{
		IIARActionState* actionInterface = InterfaceCast<IIARActionState>(Abilities->ActionBarOne[1].Ability.Get());
		if (actionInterface)
		{
			actionInterface->InputPressed();
		}

		//UARActionStateComponent* comp = ActionButtonOne->FindComponentByClass<UARActionStateComponent>();

		//if (comp)
		//{
		//ActionButtonOne->InputPressed();
		//}
	}
}

void AARCharacter::PickupItem()
{
	Equipment->PickupItem();
}

void AARCharacter::InputSwapLeftWeapon()
{
	Equipment->SwapLeftWeapon();
}
void AARCharacter::InputSwapRightWeapon()
{
	Equipment->SwapRightWeapon();
}


void AARCharacter::InputFireLeftWeapon()
{
	if (Equipment->ActiveLeftHandWeapon)
	{
		IIARActionState* actionInterface = InterfaceCast<IIARActionState>(Equipment->ActiveLeftHandWeapon);
		if (actionInterface)
		{
			actionInterface->InputPressed();
		}
	}
}
void AARCharacter::InputFireRightWeapon()
{
	if (Equipment->ActiveRightHandWeapon)
	{
		IIARActionState* actionInterface = InterfaceCast<IIARActionState>(Equipment->ActiveRightHandWeapon);
		if (actionInterface)
		{
			actionInterface->InputPressed();
		}
	}
}
void AARCharacter::InputStopFireLeftWeapon()
{
	if (Equipment->ActiveLeftHandWeapon)
	{
		IIARActionState* actionInterface = InterfaceCast<IIARActionState>(Equipment->ActiveLeftHandWeapon);
		if (actionInterface)
		{
			actionInterface->InputReleased();
		}
	}
}
void AARCharacter::InputStopFireRightWeapon()
{
	if (Equipment->ActiveRightHandWeapon)
	{
		IIARActionState* actionInterface = InterfaceCast<IIARActionState>(Equipment->ActiveRightHandWeapon);
		if (actionInterface)
		{
			actionInterface->InputReleased();
		}
	}
}


float AARCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	return 0;
}