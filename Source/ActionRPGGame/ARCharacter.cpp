// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "Abilities/ARPAbility.h"
#include "ActionState/IARActionState.h"
#include "ActionState/ARActionStateComponent.h"
#include "Componenets/ARAttributeComponent.h"
#include "Componenets/AREquipmentComponent.h"
#include "Items/ARWeapon.h"
#include "ARPlayerController.h"

#include "Net/UnrealNetwork.h"

#include "ARCharacter.h"

//////////////////////////////////////////////////////////////////////////
// AActionRPGGameCharacter

AARCharacter::AARCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// Set size for collision capsule
	CapsuleComponent->InitCapsuleSize(42.f, 96.0f);

	//LowOffset = FVector(-380.0f, 35.0f, 15.0f);
	//MidOffset = FVector(-380.0f, 35.0f, 60.0f);
	//HighOffset = FVector(-380.0f, 35.0f, 150.0f); //x,y,z

	CameraBoom = PCIP.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
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
	Attributes->GetNetAddressable();
	Attributes->SetIsReplicated(true);

	Equipment = PCIP.CreateDefaultSubobject<UAREquipmentComponent>(this, TEXT("Equipment"));
	Equipment->Activate();
	Equipment->bAutoRegister = true;
	Equipment->GetNetAddressable();
	Equipment->SetIsReplicated(true);


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


	InventoryVisibility = EVisibility::Collapsed;

}

void AARCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (Role == ROLE_Authority)
	{
		OnCharacterInitialize();
		SpawnDefaultAbility();
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
		AARPAbility* abilityTemp = GetWorld()->SpawnActor<AARPAbility>(AbilityToUse, SpawnInfo);
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
}
void AARCharacter::PossessedBy(class AController* NewController)
{
	Super::PossessedBy(NewController);
	Equipment->TargetCharacter = this;
	Equipment->TargetController = Cast<AARPlayerController>(GetController());
}


void AARCharacter::EquipAbility(class AARPAbility* AbilityIn)
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

void AARCharacter::ServerEquipAbility_Implementation(class AARPAbility* AbilityIn)
{
	EquipAbility(AbilityIn);
}

bool AARCharacter::ServerEquipAbility_Validate(class AARPAbility* AbilityIn)
{
	return true;
}

void AARCharacter::SetCurrentAbility(class AARPAbility* AbilityIn)
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
	InputComponent->BindAction("ActionButtonOne", IE_Pressed, this, &AARCharacter::InputActionButtonOne);

	InputComponent->BindAction("PickupItem", IE_Pressed, this, &AARCharacter::PickupItem);
	InputComponent->BindAction("ShowInventory", IE_Pressed, this, &AARCharacter::SetInventoryVisibility);

	//Add weapon switch for left and right hand.
	InputComponent->BindAction("SwapLeftWeapon", IE_Pressed, this, &AARCharacter::InputSwapLeftWeapon);
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
void AARCharacter::InputActionButtonOne()
{
	if (ActionButtonOne)
	{
		IIARActionState* actionInterface = InterfaceCast<IIARActionState>(ActionButtonOne);
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

void AARCharacter::SetInventoryVisibility()
{
	if (InventoryVisibility == EVisibility::Visible)
	{
		InventoryVisibility = EVisibility::Collapsed;
	}
	else if (InventoryVisibility == EVisibility::Collapsed)
	{
		InventoryVisibility = EVisibility::Visible;
	}
}
EVisibility AARCharacter::GetInventoryVisibility()
{
	return InventoryVisibility;
}

void AARCharacter::InputSwapLeftWeapon()
{
	Equipment->SwapLeftWeapon();
}

void AARCharacter::ServerSwapLeftWeapon_Implementation()
{

}
bool AARCharacter::ServerSwapLeftWeapon_Validate()
{
	return true;
}

//void AARCharacter::CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult)
//{
//	Super::CalcCamera(DeltaTime, OutResult);
//	FVector Loc, Pos, HitLocation, HitNormal, EyeLoc, FinalPos;
//	FRotator Rot, EyeRot;
//	FPOV OrigPOV;
//	//It should be header file, but for now it will work.
//	//FVector LowOffset = FVector(-380.0f,35.0f, 15.0f);
//	//FVector MidOffset = FVector(-380.0f,35.0f, 60.0f);
//	//FVector HighOffset = FVector(-380.0f,35.0f,150.0f); //x,y,z
//
//	//for some reason using blueprint Offset doesn't work.
//	//so we redecalre it locally and assign value trough blueprint.
//	FVector LowOffset = this->LowOffset;
//	FVector MidOffset = this->MidOffset;
//	FVector HighOffset = this->HighOffset; //x,y,z
//	//APlayerController* PC;
//	//int32 sizeX, sizeY;
//
//	//if(OutVT.Target)
//	//{
//	//	PC = Cast<APlayerController>(OutVT.Target->GetInstigatorController());
//	//	PC->GetViewportSize(sizeX, sizeY);
//	//	float aspectRatio = 0.0f;
//	//	aspectRatio = (float)sizeX / (float)sizeY;
//	//	if(aspectRatio >= 1.333f && aspectRatio < 1.777f)
//	//	{
//	//		LowOffset = FVector(-100.0f,10.0f, 5.0f);
//	//		MidOffset = FVector(-100.0f,10.0f, 0.0f);
//	//		HighOffset = FVector(-110.0f,10.0f,80.0f); 
//	//	}
//	//	else if (aspectRatio > 1.76f)
//	//	{
//	//		LowOffset = FVector(-120.0f,10.0f, 5.0f);
//	//		MidOffset = FVector(-200.0f,10.0f, 0.0f);
//	//		HighOffset = FVector(-190.0f,10.0f,70.0f); 
//	//	}
//	//	else
//	//	{
//	//		LowOffset = FVector(-120.0f,10.0f, 5.0f);
//	//		MidOffset = FVector(-200.0f,10.0f, 0.0f);
//	//		HighOffset = FVector(-180.0f,10.0f,80.0f); 
//	//	}
//	//}
//
//
//	FVector FinalOffset = MidOffset;
//
//	float pitchXX = 0;
//	float pitchZZ = 0;
//	//OrigPOV = OutVT.POV;
//	//OutVT.Target->GetActorEyesViewPoint(EyeLoc, EyeRot);
//	this->GetActorEyesViewPoint(EyeLoc, EyeRot);
//	Loc = this->GetActorLocation();
//	//normalize axis to space between -180 and 180,
//	// we need because by default rotator is between 0 and 360
//	float normalizedEyeRotPitch = EyeRot.NormalizeAxis(EyeRot.Pitch);
//	//we do everything in Absolute numbers and then just multipli by -/+1
//	//looking up
//	if (EyeRot.NormalizeAxis(EyeRot.Pitch) > 0.0f)
//	{
//		//FinalOffset = LowOffset;
//		float offsetX = FMath::Abs(LowOffset.X) / LowOffset.X;
//		float offsetZ = FMath::Abs(LowOffset.Z) / LowOffset.Z;
//
//		//HighOffset.X = math.Abs(HighOffset.X);
//		//HighOffset.Z = math.Abs(HighOffset.Z);
//		LowOffset = LowOffset.GetAbs();
//		MidOffset = MidOffset.GetAbs();
//		FinalOffset = FinalOffset.GetAbs(); //we convert to absolute number everything is +
//
//		float pitchX = (LowOffset.X - MidOffset.X) / 60; //-?
//		pitchXX = pitchX * normalizedEyeRotPitch; // -
//		pitchXX = FMath::Abs(pitchXX); // make sure result is +
//		float pitchZ = (LowOffset.Z - MidOffset.Z) / 60; //180 / -60 //-
//		pitchZZ = pitchZ * normalizedEyeRotPitch; // -
//		pitchZZ = FMath::Abs(pitchZZ); //make sure it is +
//
//		FinalOffset.X = FinalOffset.X + (pitchXX);
//		FinalOffset.X = FMath::Clamp(FinalOffset.X, LowOffset.X, MidOffset.X) * offsetX;//we have issue here what if the values will be positive ?
//		FinalOffset.Z = FinalOffset.Z + (pitchZZ);
//		if (LowOffset.Z > MidOffset.Z)//guess other offset adjustments also would need that.
//		{
//			FinalOffset.Z = FMath::Clamp(FinalOffset.Z, MidOffset.Z, LowOffset.Z) * offsetZ;
//		}
//		else
//		{
//			FinalOffset.Z = FMath::Clamp(FinalOffset.Z, LowOffset.Z, MidOffset.Z) * offsetZ;
//		}
//		FinalOffset.Y = LowOffset.Y;
//	} //looking down
//	else if (EyeRot.NormalizeAxis(EyeRot.Pitch) < 0.0f)
//	{
//		float offsetX = FMath::Abs(HighOffset.X) / HighOffset.X;
//		float offsetZ = FMath::Abs(HighOffset.Z) / HighOffset.Z;
//
//		//HighOffset.X = math.Abs(HighOffset.X);
//		//HighOffset.Z = math.Abs(HighOffset.Z);
//		HighOffset = HighOffset.GetAbs();
//		MidOffset = MidOffset.GetAbs();
//		FinalOffset = FinalOffset.GetAbs(); //we convert to absolute number everything is +
//
//		float pitchX = (HighOffset.X - MidOffset.X) / -60; //-?
//		pitchXX = pitchX * normalizedEyeRotPitch; // -
//		pitchXX = FMath::Abs(pitchXX); // make sure result is +
//		float pitchZ = (HighOffset.Z - MidOffset.Z) / -60; //180 / -60 //-
//		pitchZZ = pitchZ * normalizedEyeRotPitch; // -
//		pitchZZ = FMath::Abs(pitchZZ);
//
//		FinalOffset.X = FinalOffset.X + (pitchXX);
//		FinalOffset.X = FMath::Clamp(FinalOffset.X, MidOffset.X, HighOffset.X) * offsetX;//we have issue here what if the values will be positive ?
//		FinalOffset.Z = FinalOffset.Z + (pitchZZ);
//		FinalOffset.Z = FMath::Clamp(FinalOffset.Z, MidOffset.Z, HighOffset.Z) * offsetZ;
//		FinalOffset.Y = HighOffset.Y;
//	} //looking directly straight pitch = 0, dunno if that ever happen.
//	else
//	{
//		FinalOffset = MidOffset;
//	}
//
//	FinalPos = EyeLoc;
//	//FinalOffset = FinalOffset.SafeNormal();
//	FinalPos += FRotationMatrix(EyeRot).TransformVector(FinalOffset);
//	//FinalPos += FRotationMatrix(EyeRot).TransformPosition(FinalOffset);
//	OutResult.Location = FinalPos;
//	OutResult.Rotation = EyeRot;
//	OutResult.AspectRatio = 1.7777779;
//	OutResult.bConstrainAspectRatio = true;
//	/*OutVT.POV.Location = FinalPos;
//	OutVT.POV.Rotation = EyeRot;*/
//}