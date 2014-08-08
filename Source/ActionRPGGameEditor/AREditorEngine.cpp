// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGameEditor.h"
//#include "../ActionRPGGame/Types/ARStructTypes.h"
#include "GameplayTagsModule.h"
#include "AREditorEngine.h"

UAREditorEngine::UAREditorEngine(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}

void UAREditorEngine::Init(IEngineLoop* InEngineLoop)
{
	Super::Init(InEngineLoop);

	//DataTable'/Game/Blueprints/Data/GameTags.GameTags'
	IGameplayTagsModule& GameplayTagsModule = IGameplayTagsModule::Get();
	FString Tags = "DataTable'/Game/Blueprints/Data/GameTags.GameTags'";

	////UDataTable* justDeleteMe = LoadObject<UDataTable>(NULL, *Tags, NULL, LOAD_None, NULL);

	TArray<FString> TagsList;
	TagsList.Add(Tags);
	GameplayTagsModule.GetGameplayTagsManager().LoadGameplayTagTables(TagsList);
}
