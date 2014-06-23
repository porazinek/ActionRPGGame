// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"
#include "Types/ARStructTypes.h"
#include "AREditorEngine.h"

UAREditorEngine::UAREditorEngine(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}

void UAREditorEngine::Init(IEngineLoop* InEngineLoop)
{
	Super::Init(InEngineLoop);
	FString table = "DataTable'/Game/Blueprints/Data/ChestItemData.ChestItemData'";
	ChestItemDataTable = LoadObject<UDataTable>(NULL, *table, NULL, LOAD_None, NULL);
}
