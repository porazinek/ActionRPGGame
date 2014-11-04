// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "ARWeaponComponent.generated.h"

/*
	Base class for weapon componenets.
	Weapon componenets can live as name implies, inside AWeapon.
	They are base building blocks for creating weapons.

	I have only bit clouded idea of how it is supposed to work in the end.
	There will be State Componenet, responsible for handling things like reloading, shooting etc.
	Component for handling 
*/
UCLASS(hidecategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming), editinlinenew, meta = (BlueprintSpawnableComponent))
class UARWeaponComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

};
