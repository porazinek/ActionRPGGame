// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "ARPlayerController.h"

#include "ARPGameMode.h"

AARPGameMode::AARPGameMode(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FObjectFinder<UClass> PlayerPawnBPClass(TEXT("Class'/Game/Blueprints/MyCharacter2.MyCharacter2_C'"));
	//static ConstructorHelpers::FObjectFinder<UClass> PlayerControllerBPClass(TEXT("Class'/Game/Blueprints/BP_PlayerController.BP_PlayerController_C'"));
	static ConstructorHelpers::FObjectFinder<UBlueprint> TheHUDOb(TEXT("Blueprint'/Game/Blueprints/HUD/BP_HUD.BP_HUD'"));

	if (TheHUDOb.Object != NULL)
	{
		HUDClass = (UClass*)TheHUDOb.Object->GeneratedClass;
	}
	if (PlayerPawnBPClass.Object != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Object;
	}
	//if (PlayerControllerBPClass.Object != NULL)
	//{
	//	PlayerControllerClass = PlayerControllerBPClass.Object;
	//}

	//Blueprint'/Game/Blueprints/MyCharacter.MyCharacter'
	//Blueprint'/Game/Blueprints/BP_PlayerController.BP_PlayerController'
	PlayerControllerClass = AARPlayerController::StaticClass();
}
