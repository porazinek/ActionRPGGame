// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "ARAbilityBaseComponent.h"

UARAbilityBaseComponent::UARAbilityBaseComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = true;
	bAutoRegister = true;

	bWantsInitializeComponent = true;
}