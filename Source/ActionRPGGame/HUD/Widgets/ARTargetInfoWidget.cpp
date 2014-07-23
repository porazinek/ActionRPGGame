// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "ActionRPGGame.h"

#include "../../Types/ARStructTypes.h"
#include "../../ARPlayerController.h"

#include "../../Componenets/ARAttributeComponent.h"
#include "../../ARCharacter.h"

#include "../../Abilities/ARAbility.h"
#include "../../ActionState/ARActionStateComponent.h"

#include "ARHealthBarWidget.h"
#include "ARTargetInfoWidget.h"


void SARTargetInfoWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;
	MyPC = InArgs._MyPC;
	MyAttrComp = InArgs._MyAttrComp;
	HealthBarWidth = InArgs._HealthBarWidth;
	HealthBarHeight = InArgs._HealthBarHeight;
	HealthBarPosX = InArgs._HealthBarPosX;
	HealthBarPosY = InArgs._HealthBarPosY;
	HealthBarPosition.Bind(this, &SARTargetInfoWidget::GetHealthPosition);
	HealthBarSize.Bind(this, &SARTargetInfoWidget::GetHealthSize);
	CurrentHealth.Bind(this, &SARTargetInfoWidget::GetCurrentHealth);
	MaxHealth.Bind(this, &SARTargetInfoWidget::GetMaxHealth);

	ChildSlot
		[
		/*	SNew(SOverlay)
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[*/
				SNew(SCanvas)
				+ SCanvas::Slot()
				.Position(HealthBarPosition)
				.Size(HealthBarSize)
				//.Padding(FMargin(HealthBarPosX, HealthBarPosY, 0, 0))
				[
					//SNew(SBox)
					//.WidthOverride(HealthBarWidth)
					//.HeightOverride(HealthBarHeight)
					//[
					SNew(SARHealthBarWidget)
					.MyPC(MyPC)
					.OwnerHUD(OwnerHUD)
					.MyAttrComp(MyAttrComp)
					.CurrentValue(CurrentHealth)
					.MaxValue(MaxHealth)
					//]
				]
			//]
		];
}

FVector2D SARTargetInfoWidget::GetHealthPosition() const
{
	return FVector2D(HealthBarPosX, HealthBarPosY);
}
FVector2D SARTargetInfoWidget::GetHealthSize() const
{
	return FVector2D(HealthBarWidth, HealthBarHeight);
}
float SARTargetInfoWidget::GetCurrentHealth() const
{
	if (MyAttrComp.IsValid())
	{
		return MyAttrComp->Health;
	}
	return 0;
}
float SARTargetInfoWidget::GetMaxHealth() const
{
	if (MyAttrComp.IsValid())
	{
		return MyAttrComp->MaxHealth;
	}
	return 0;
}