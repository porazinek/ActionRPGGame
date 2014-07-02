// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "Engine.h"
#include "UnrealEd.h"
#include "AREdEngine.generated.h"

UCLASS()
class UAREdEngine : public UUnrealEdEngine
{
	GENERATED_UCLASS_BODY()


		virtual void Init(IEngineLoop* InEngineLoop) override;
};



