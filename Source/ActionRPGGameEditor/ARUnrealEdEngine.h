// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
//#include "Engine.h"
//#include "Editor/UnrealEd/Classes/Editor/UnrealEdEngine.h"
//#include "UnrealEd.h"
//#include "Editor/UnrealEdEngine.h"
#include "UnrealEd.h"
#include "ARUnrealEdEngine.generated.h"

UCLASS()
class UARUnrealEdEngine : public UUnrealEdEngine
{
	GENERATED_UCLASS_BODY()

	virtual void Init(IEngineLoop* InEngineLoop) override;
};



