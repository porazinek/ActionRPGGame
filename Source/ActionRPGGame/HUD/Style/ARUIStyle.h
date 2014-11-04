// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "SlateBasics.h"

class FARUIStyle
{
public:
	static void Initialize();
	static void Shutdown();

	static void ReloadTextures();
	static const ISlateStyle& Get();
	static FName GetStyleName();
protected:
	
private:
	static TSharedRef<class FSlateStyleSet> Create();
	static TSharedPtr<class FSlateStyleSet> ARStyleInstance;
};

