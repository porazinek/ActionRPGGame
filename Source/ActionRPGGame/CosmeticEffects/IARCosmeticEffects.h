// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "IARCosmeticEffects.generated.h"

/*
	Will work with conjustion to cosmetic Effect componenets, in this folder.
	If actor will want to use any of these component, it must implement this interface.
	It will contain set of standard functions, for retriving various data (like location, 
	socket names, etc).
*/

UINTERFACE(MinimalAPI)
class UIARCosmeticEffects : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IIARCosmeticEffects
{
	GENERATED_IINTERFACE_BODY()
public:
	UFUNCTION()
		virtual FVector GetOriginLocation();
};
