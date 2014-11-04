// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "IARInputAction.generated.h"

UINTERFACE(MinimalAPI)
class UIARInputAction : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IIARInputAction
{
	GENERATED_IINTERFACE_BODY()

	/*[Client]*/
	UFUNCTION()
		virtual void InputPressed();
	/*[Client]*/
	UFUNCTION()
		virtual void InputReleased();
};
