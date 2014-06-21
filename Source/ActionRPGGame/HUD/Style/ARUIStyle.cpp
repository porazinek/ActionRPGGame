// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "SlateGameResources.h"

#include "ARUIStyle.h"

TSharedPtr<FSlateStyleSet> FARUIStyle::ARStyleInstance = NULL;

void FARUIStyle::Initialize()
{
	if (!ARStyleInstance.IsValid())
	{
		ARStyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*ARStyleInstance);
	}
}

void FARUIStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*ARStyleInstance);
	ensure(ARStyleInstance.IsUnique());
	ARStyleInstance.Reset();
}

FName FARUIStyle::GetStyleName()
{
	static FName StyleSetName(TEXT("ARUIStyle"));
	return StyleSetName;
}

TSharedRef<FSlateStyleSet> FARUIStyle::Create()
{
	TSharedRef<FSlateStyleSet> StyleRef = FSlateGameResources::New(FARUIStyle::GetStyleName(), "/Game/UI/Styles", "/Game/UI/Styles");
	FSlateStyleSet& Set = StyleRef.Get();

	return StyleRef;
}


void FARUIStyle::ReloadTextures()
{
	FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
}

const ISlateStyle& FARUIStyle::Get()
{
	return *ARStyleInstance;
}