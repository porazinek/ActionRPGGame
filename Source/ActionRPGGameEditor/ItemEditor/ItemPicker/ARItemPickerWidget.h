// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "ActionRPGGameEditor.h"
#include "SlateCore.h"
#include "Slate.h"


class SARItemPickerWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SARItemPickerWidget)
	{}
		SLATE_ARGUMENT(FARItemPickerContainer*, ItemContainer)
	SLATE_END_ARGS()

public:
	
	void Construct(const FArguments& InArgs);

	TArray<TSharedPtr<FString>> ItemNames;

	TSharedRef<ITableRow> MakeListItems(TSharedPtr<FString> Item, const TSharedRef<STableViewBase>& OwnerTable);

	FARItemPickerContainer* ItemContainer;
};


