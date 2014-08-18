// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "ActionRPGGame.h"

#include "../../Types/ARStructTypes.h"
#include "../../ARPlayerController.h"

#include "../../Abilities/ARAbility.h"
#include "../../ActionState/ARActionStateComponent.h"
#include "../../Componenets/ARAttributeBaseComponent.h"
#include "../../Items/ARInventoryComponent.h"

#include "ARInventoryWidget.h"
#include "ARInventoryItemWidget.h"
#include "ARCharacterSheetWidget.h"
#include "ARActionBarWidget.h"
#include "ARAbilityInventoryWidget.h"
#include "ARCastingBarWidget.h"
#include "ARFloatingCombatTextWidget.h"
#include "ARResourceWidget.h"
#include "ARTargetInfoWidget.h"

#include "ARLootingWidget.h"

#include "ARHUDWidget.h"


void SARHUDWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;
	MyPC = InArgs._MyPC;
	MyChar = InArgs._MyChar;
	MyAttrComp = InArgs._MyAttrComp;
	CastbarWidth = InArgs._CastbarWidth;
	CastbarHeight = InArgs._CastbarHeight;
	CastbarPositionX = InArgs._CastbarPositionX;
	CastbarPositionY = InArgs._CastbarPositionY;

	//MyAttrComp->OnInstigatorDamage.AddRaw(this, &SARHUDWidget::CreateFCTWidget);
	ChildSlot
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.Padding(FMargin(CastbarPositionX, CastbarPositionY, 0, 0))
			[
				SNew(SBox)
				.HeightOverride(CastbarHeight)
				.WidthOverride(CastbarWidth)
				[
					SNew(SARCastingBarWidget)
					.OwnerHUD(OwnerHUD)
					.MyPC(MyPC)
				]
			]
			+ SOverlay::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Top)
				[
					SNew(SBox)
					//.HeightOverride(CastbarHeight)
					.WidthOverride(200)
					[
						SNew(SARResourceWidget)
						.OwnerHUD(OwnerHUD)
						.MyPC(MyPC)
						.MyAttrComp(MyAttrComp)
					]
				]
			//+ SOverlay::Slot()
			//	.HAlign(HAlign_Fill)
			//	.VAlign(VAlign_Fill)
			//	[
			//		//SAssignNew(FCTCanvas, SCanvas)
			//		SNew(SCanvas)
			//		+ SCanvas::Slot()
			//		.Position(FVector2D(600, 300))
			//		.Size(FVector2D(400,400))
			//		[
			//			SNew(SBox)
			//			//.Visibility(TAttribute<EVisibility>(this, &SARHUDWidget::GetFCTVisibility))
			//			[
			//				SNew(SARTargetInfoWidget)
			//				.OwnerHUD(OwnerHUD)
			//				.MyPC(MyPC)
			//				.MyAttrComp(TAttribute<TWeakObjectPtr<UARAttributeComponent>>(this, &SARHUDWidget::GetTargetAttrComp))
			//			]
			//		]
			//	]
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
						.Character(MyChar)
						.InventoryColumns(5)
						.InventoryRows(10)
						.Inventory(MyPC->Inventory.Get())
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
							.Character(MyChar)
							.Equipment(MyChar->Equipment.Get())
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
							.AbilityComp(MyPC->Abilities.Get())
						]
					]
				]
			+ SOverlay::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Top)
				.Padding(FMargin(600, 0, 0, 0))
				[
					SNew(SBorder) //add visibility check
					.BorderBackgroundColor(FSlateColor(FLinearColor(1, 0, 0, 1)))
					.Visibility(this, &SARHUDWidget::GetAbilityInventoryVisibility)
					[
						SNew(SBox)
						.HeightOverride(400)
						.WidthOverride(400)
						[
							SNew(SARAbilityInventoryWidget)
							.OwnerHUD(OwnerHUD)
							.MyPC(MyPC)
							.AbilityComp(MyPC->Abilities.Get())
						]
					]
				]
			//Looting Widget (Window)
			//+ SOverlay::Slot()
			//	.HAlign(HAlign_Right)
			//	.VAlign(VAlign_Top)
			//	.Padding(FMargin(600, 0, 0, 0))
			//	[
			//		SNew(SBorder) //add visibility check
			//		.BorderBackgroundColor(FSlateColor(FLinearColor(1, 0, 0, 1)))
			//	//	.Visibility(this, &SARHUDWidget::GetAbilityInventoryVisibility)
			//		[
			//			SNew(SBox)
			//			.HeightOverride(400)
			//			.WidthOverride(400)
			//			[
			//				SNew(SARLootingWidget)
			//				.MyPC(MyPC)
			//				.Character(MyChar)
			//				.Equipment(MyChar->Equipment.Get())
			//				.Inventory(MyPC->Inventory.Get())
			//			]
			//		]
			//	]
		];
}
TWeakObjectPtr<class UARAttributeComponent> SARHUDWidget::GetTargetAttrComp() const
{
	return TargetAttrComp.Get();
}
void SARHUDWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
	//for (int32 Index = 0; Index < FCTBox->GetNumWidgets(); Index++)
	//{
	//	//FCTBox->GetChildren()->GetChildAt(Index);
	//}
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

EVisibility SARHUDWidget::GetTargetResourceVisibility() const
{
	return TargetResourceVisibility;
}
void SARHUDWidget::SetTargetResourceVisibility(EVisibility ValueIn)
{
	TargetResourceVisibility = ValueIn;
}

