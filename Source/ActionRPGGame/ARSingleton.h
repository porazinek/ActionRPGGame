// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "ARSingleton.generated.h"

UCLASS(Config=Game, notplaceable)
class UARSingleton : public UObject, public FTickerObjectBase
{
	GENERATED_UCLASS_BODY()

private:
	UARSingleton();

public:
	static UARSingleton& Get();
	FStreamableManager AssetLoader;

	virtual bool Tick(float DeltaSeconds) override;
};



