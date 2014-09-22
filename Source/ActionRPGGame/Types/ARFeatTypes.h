// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "AREnumTypes.h"
#include "ARFeatTypes.generated.h"

/*
	Very game specific structs

	Feats (or traits, perks, I dunno what the finall name might be), are objects
	which are active for entire game duration, and exist in attribute component.
	They usually listen for incoming events, from other objects, and modify their properties.

	What exactly they modify might vary widely, depending on incoming object type.

	In online play, feats exist only on server, while client will only display information
	about what feat does.
*/

USTRUCT(BlueprintType)
struct FARFeatData
{
	GENERATED_USTRUCT_BODY()
public:
	//this will indicate if we should spawn upgrade and add it to ability, 
	//while it become active (spawned)
	//No need to spawn all objects just to check if they should be spawned in first place ;)
	UPROPERTY(EditAnywhere, Category = "Upgrade Active")
		bool IsFeatEquiped;

	//Add TSubclassOf for actuall feat/effect/whatever which will spawned when it's going to be active.
	//
};
