// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "ActionRPGGame.h"

#include "../../Types/ARStructTypes.h"
#include "../../ARPlayerController.h"

#include "ARInventoryWidget.h"
#include "ARInventoryItemWidget.h"
#include "ARCharacterSheetWidget.h"
#include "ARActionBarWidget.h"

#include "ARHUDWidget.h"


void SARHUDWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;
	MyPC = InArgs._MyPC;
	InventoryVisibility = EVisibility::Visible;
	ChildSlot
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Top)
			[
				SNew(SBorder) //add visibility check
				.BorderBackgroundColor(FSlateColor(FLinearColor(1, 0, 0, 1)))
				.Visibility(this, &SARHUDWidget::GetInventoryVisibility)
				[
					SNew(SBox)
					.WidthOverride(320)
					.HeightOverride(640)
					//.Content(640)
					[
						SNew(SARInventoryWidget)
						.PlayerController(MyPC)
						.InventoryColumns(5)
						.InventoryRows(10)
					]
				]
			]
			//Character Sheet;
			+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Bottom)
				[
					SNew(SBorder) //add visibility check
					.BorderBackgroundColor(FSlateColor(FLinearColor(1, 0, 0, 1)))
					[
						SNew(SBox)
						[
							SNew(SARCharacterSheetWidget)
							.MyPC(MyPC)
						]
					]
				]
			+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Bottom)
				[
					SNew(SBorder) //add visibility check
					.BorderBackgroundColor(FSlateColor(FLinearColor(1, 0, 0, 1)))
					[
						SNew(SBox)
						[
							SNew(SARActionBarWidget)
							.OwnerHUD(OwnerHUD)
							.MyPC(MyPC)
						]
					]
				]
		];
}

EVisibility SARHUDWidget::GetInventoryVisibility() const
{
	if (MyPC.IsValid())
	{
		return MyPC->InventoryVisibility;
	}
	return EVisibility::Collapsed;
}