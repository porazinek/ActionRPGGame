// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"
#include "../Types/ARStructTypes.h"
#include "ARItem.h"

AARItem::AARItem(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	bReplicates = true;
	bNetUseOwnerRelevancy = true;
}
