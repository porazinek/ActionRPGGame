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
#include "AREnergyBarWidget.h"
#include "ARStaminaBarWidget.h"

#include "ARResourceWidget.h"

void SARResourceWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;
	MyPC = InArgs._MyPC;
	MyAttrComp = InArgs._MyAttrComp;
	CurrentHealth.Bind(this, &SARResourceWidget::GetCurrentHealth);
	MaxHealth.Bind(this, &SARResourceWidget::GetMaxHealth);
	ChildSlot
		[
			
			SNew(SUniformGridPanel)
			+ SUniformGridPanel::Slot(0, 0)
			[
				SNew(SBox)
				//.WidthOverride(100)
				.HeightOverride(20)
				[
					SNew(SARHealthBarWidget)
					.MyPC(MyPC)
					.OwnerHUD(OwnerHUD)
					.MyAttrComp(MyAttrComp)
					.CurrentValue(CurrentHealth)
					.MaxValue(MaxHealth)
				]
			]
			+ SUniformGridPanel::Slot(0, 1)
				[
					SNew(SBox)
					//.WidthOverride(100)
					.HeightOverride(20)
					[
						SNew(SAREnergyBarWidget)
						.MyPC(MyPC)
						.OwnerHUD(OwnerHUD)
						.MyAttrComp(MyAttrComp)
					]
				]
			+ SUniformGridPanel::Slot(0, 2)
				[
					SNew(SBox)
					//.WidthOverride(100)
					.HeightOverride(20)
					[
						SNew(SARStaminaBarWidget)
						.MyPC(MyPC)
						.OwnerHUD(OwnerHUD)
						.MyAttrComp(MyAttrComp)
					]
				]
		];
}

float SARResourceWidget::GetCurrentHealth() const
{
	if (MyAttrComp.IsValid())
	{
		return MyAttrComp->Health;
	}
	return 0;
}
float SARResourceWidget::GetMaxHealth() const
{
	if (MyAttrComp.IsValid())
	{
		return MyAttrComp->MaxHealth;
	}
	return 0;
}

float SARResourceWidget::GetCurrentEnergy() const
{
	return 0;
}
float SARResourceWidget::GetMaxEnergy() const
{
	return 0;
}

float SARResourceWidget::GetCurrentStamina() const
{
	return 0;
}
float SARResourceWidget::GetMaxStamina() const
{
	return 0;
}