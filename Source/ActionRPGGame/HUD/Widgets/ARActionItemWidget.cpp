// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "ActionRPGGame.h"

#include "../../Types/ARStructTypes.h"
#include "../../ARPlayerController.h"
#include "../../Abilities/ARAbility.h"

#include "ARCharacterSheetWidget.h"

#include "ARActionItemWidget.h"


void SARActionItemWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;
	MyPC = InArgs._MyPC;
	CurrentAbility = InArgs._CurrentAbility;

	ChildSlot
		[
			SNew(SBox)
			.WidthOverride(50)
			.HeightOverride(50)
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				[
					SNew(STextBlock)
					.ShadowColorAndOpacity(FLinearColor::Black)
					.ColorAndOpacity(FLinearColor::White)
					.ShadowOffset(FIntPoint(-1, 1))
					.Font(FSlateFontInfo("Veranda", 16))
					.Text(this, &SARActionItemWidget::GetCurrentCooldown)
				]
				+ SOverlay::Slot()
					[
						SNew(STextBlock)
						.Text(FText::FromName("Buton"))
					]
			]
		];
}

FText SARActionItemWidget::GetCurrentCooldown() const
{
	if (CurrentAbility->Ability.IsValid())
	{
		if (CurrentAbility->Ability->IsOnCooldown)
		{
			return FText::AsNumber(CurrentAbility->Ability->CurrentCooldownTime);
		}
	}
	return FText::FromName("");
}