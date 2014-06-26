// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "ActionRPGGame.h"

#include "../../Types/ARStructTypes.h"
#include "../../ARPlayerController.h"

#include "ARInventoryWidget.h"
#include "ARInventoryItemWidget.h"
#include "ARCharacterSheetWidget.h"
#include "ARActionBarWidget.h"
#include "ARAbilityInventoryWidget.h"

#include "ARHUDWidget.h"


void SARHUDWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;
	MyPC = InArgs._MyPC;

	ChildSlot
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
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
			+ SOverlay::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Top)
				.Padding(FMargin(300, 0, 0, 0))
				[
					SNew(SBorder) //add visibility check
					.BorderBackgroundColor(FSlateColor(FLinearColor(1, 0, 0, 1)))
					.Visibility(this, &SARHUDWidget::GetCharacterSheetVisibility)
					[
						SNew(SBox)
						[
							SNew(SARCharacterSheetWidget)
							.MyPC(MyPC)
						]
					]
				]
			+ SOverlay::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Bottom)
				[
					SNew(SBorder) //add visibility check
					.BorderBackgroundColor(FSlateColor(FLinearColor(1, 0, 0, 1)))
					[
						SNew(SBox)
						.HeightOverride(56)
						.WidthOverride(500)
						[
							SNew(SARActionBarWidget)
							.OwnerHUD(OwnerHUD)
							.MyPC(MyPC)
						]
					]
				]
			+ SOverlay::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Top)
				[
					SNew(SBorder) //add visibility check
					.BorderBackgroundColor(FSlateColor(FLinearColor(1, 0, 0, 1)))
					.Visibility(this, &SARHUDWidget::GetAbilityInventoryVisibility)
					[
						SNew(SBox)
						.HeightOverride(300)
						.WidthOverride(500)
						[
							SNew(SARAbilityInventoryWidget)
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

EVisibility SARHUDWidget::GetAbilityInventoryVisibility() const
{
	if (MyPC.IsValid())
	{
		return MyPC->AbilityInventoryVisibility;
	}
	return EVisibility::Collapsed;
}
EVisibility SARHUDWidget::GetCharacterSheetVisibility() const
{
	if (MyPC.IsValid())
	{
		return MyPC->CharacterSheetVisibility;
	}
	return EVisibility::Collapsed;
}