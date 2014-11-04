// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"
#include "ARSingleton.h"

UARSingleton::UARSingleton(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

UARSingleton& UARSingleton::Get()
{
	UARSingleton* Singleton = Cast<UARSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}
	else
	{
		return *ConstructObject<UARSingleton>(UARSingleton::StaticClass());
	}
}

bool UARSingleton::Tick(float DeltaSeconds)
{
	return true; //nothing going on.
}
