// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "ActionRPGGame.h"

#include "../../Types/ARStructTypes.h"
#include "../../ARPlayerController.h"

#include "ARCharacterSheetWidget.h"

#include "ARActionItemWidget.h"


void SARActionItemWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;
	MyPC = InArgs._MyPC;
	ChildSlot
		[
			SNew(SBox)
			.WidthOverride(50)
			.HeightOverride(50)
			[
				SNew(STextBlock)
				.Text(FText::FromName("Buton"))
			]
		];
}
