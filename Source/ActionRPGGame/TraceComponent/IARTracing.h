// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "IARTracing.generated.h"

UINTERFACE(MinimalAPI)
class UIARTracing : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IIARTracing
{
	GENERATED_IINTERFACE_BODY()
public:
	virtual AARCharacter* GetCharacter() { return nullptr; }
	virtual AARPlayerController* GetController() { return nullptr; }
};
