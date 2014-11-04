// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "ARWeaponComponent.h"

UARWeaponComponent::UARWeaponComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = true;
	bAutoRegister = true;

	bWantsInitializeComponent = true;
}