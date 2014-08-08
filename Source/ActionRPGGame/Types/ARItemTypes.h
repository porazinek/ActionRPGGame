// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "AREnumTypes.h"
#include "ARAttributeTypes.h"
#include "Slate.h"
#include "ARItemTypes.generated.h"



USTRUCT()
struct FEmptyStruct
{
	GENERATED_USTRUCT_BODY()
};

/*
It's struct containing info about item.
It have advantage of being simple to store in single DataAsset, but Items can't
be directly placed on level.

On other hand managing all the possible actors is not better..
*/
