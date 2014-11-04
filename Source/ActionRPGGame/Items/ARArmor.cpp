// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"
#include "../Types/ARStructTypes.h"
#include "ARArmor.h"

AARArmor::AARArmor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = false;
	bNetUseOwnerRelevancy = true;
}
