// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "Engine.h"
#include "AREngine.generated.h"

UCLASS()
class UAREngine : public UGameEngine
{
	GENERATED_UCLASS_BODY()
public:
	virtual void Init(IEngineLoop* InEngineLoop) override;
};



/* Config so i won' forget to add it to DefaultEngine.ini
GameEngine=/Script/ActionRPGGame.AREngine
EditorEngine=/Script/UnrealEd.EditorEngine
UnrealEdEngine=/Script/ActionRPGGame.AREditorEngine
*/