// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "AREffectPeriodic.generated.h"

/*
	Periodic effects are replicated back to client.
	That is because they work over period of time, and for sake of smoothnes of gameplay
	we need to make some client side predictions about effect duration.

	Still best solution would be probably to make replicated UObject. But that is way over my skill
	honestly ;p.
*/

UCLASS(minimalapi)
class AAREffectPeriodic : public AActor
{
	GENERATED_UCLASS_BODY()

};



