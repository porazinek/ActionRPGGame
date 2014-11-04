// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "ARPlayerCameraManager.h"

AARPlayerCameraManager::AARPlayerCameraManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ViewPitchMin = -70.0f;
	ViewPitchMax = 70.0f;
}
