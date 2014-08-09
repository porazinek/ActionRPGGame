// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

//UARItemsData* const FARModule::item = LoadObject<UARItemsData>(nullptr,
//	*FString("ARItemsData'/Game/Blueprints/Data/NewARItemsData.NewARItemsData'"),
//	nullptr, LOAD_None, nullptr);
void FARModule::StartupModule()
{
	FDefaultGameModuleImpl::StartupModule();
	//FARModule::item = LoadObject<UARItemsData>(nullptr,
	//	*FString("ARItemsData'/Game/Blueprints/Data/NewARItemsData.NewARItemsData'"),
	//	nullptr, LOAD_None, nullptr);

}
DEFINE_LOG_CATEGORY(AR);
IMPLEMENT_PRIMARY_GAME_MODULE(FARModule, ActionRPGGame, "ActionRPGGame");




