// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../../Types/ARStructTypes.h"

#include "ARInventoryItemWidget.h"

#include "ARCharacterSheetWidget.h"


void SARCharacterSheetWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;
	MyPC = InArgs._MyPC;

	ChildSlot
		[
			SNew(SGridPanel)
			+ SGridPanel::Slot(1, 1)
			.HAlign(HAlign_Right)
			[
				SNew(SBorder) //add visibility check
				.BorderBackgroundColor(FSlateColor(FLinearColor(1, 0, 0, 1)))
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					[
						SNew(STextBlock)
						.Text(FText::FromString(FString("LHand1")))
					]
					+ SOverlay::Slot()
						[
							SNew(SBox)
							.WidthOverride(50)
							.HeightOverride(50)
							[
								SNew(SARInventoryItemWidget)
								.SlotType(EItemSlot::Item_Weapon)
								.EquipmentSlot(EEquipmentSlot::Item_LeftHandOne)
								.PlayerController(MyPC)
								.SlotName(FText::FromName("LHand"))
							]
						]
				]
			]
			+ SGridPanel::Slot(2, 1)
				.HAlign(HAlign_Right)
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					[
						SNew(STextBlock)
						.Text(FText::FromString(FString("LHand2")))
					]
					+ SOverlay::Slot()
						[
							SNew(SBorder) //add visibility check
							.BorderBackgroundColor(FSlateColor(FLinearColor(1, 0, 0, 1)))
							[
								SNew(SBox)
								.WidthOverride(50)
								.HeightOverride(50)
								[
									SNew(SARInventoryItemWidget)
									.SlotType(EItemSlot::Item_Weapon)
									.EquipmentSlot(EEquipmentSlot::Item_LeftHandOne)
									.PlayerController(MyPC)
									.SlotName(FText::FromName("LHand"))
								]
							]
						]
				]
			+ SGridPanel::Slot(3, 1)
				.HAlign(HAlign_Right)
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					[
						SNew(STextBlock)
						.Text(FText::FromString(FString("LHand3")))
					]
					+ SOverlay::Slot()
						[
							SNew(SBorder) //add visibility check
							.BorderBackgroundColor(FSlateColor(FLinearColor(1, 0, 0, 1)))
							[
								SNew(SBox)
								.WidthOverride(50)
								.HeightOverride(50)
								[
									SNew(SARInventoryItemWidget)
									.SlotType(EItemSlot::Item_Weapon)
									.EquipmentSlot(EEquipmentSlot::Item_LeftHandOne)
									.PlayerController(MyPC)
									.SlotName(FText::FromName("LHand"))
								]
							]
						]
				]
			+ SGridPanel::Slot(4, 1)
				.HAlign(HAlign_Right)
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					[
						SNew(STextBlock)
						.Text(FText::FromString(FString("LHand4")))
					]
					+ SOverlay::Slot()
						[
							SNew(SBorder) //add visibility check
							.BorderBackgroundColor(FSlateColor(FLinearColor(1, 0, 0, 1)))
							[
								SNew(SBox)
								.WidthOverride(50)
								.HeightOverride(50)
								[
									SNew(SARInventoryItemWidget)
									.SlotType(EItemSlot::Item_Weapon)
									.EquipmentSlot(EEquipmentSlot::Item_LeftHandOne)
									.PlayerController(MyPC)
									.SlotName(FText::FromName("LHand"))
								]
							]
						]
				]
			+ SGridPanel::Slot(5, 1)
				.ColumnSpan(2)
				.HAlign(HAlign_Right)
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					[
						SNew(STextBlock)
						.Text(FText::FromString(FString("LActive")))
					]
					+ SOverlay::Slot()
						[
							SNew(SBorder) //add visibility check
							.BorderBackgroundColor(FSlateColor(FLinearColor(1, 0, 0, 1)))
							[
								SNew(SBox)
								.WidthOverride(50)
								.HeightOverride(50)
								[
									SNew(SARInventoryItemWidget)
									.SlotType(EItemSlot::Item_Weapon)
									.EquipmentSlot(EEquipmentSlot::Item_LeftHandActive)
									.PlayerController(MyPC)
									.SlotName(FText::FromName("LActive"))
								]
							]
						]
				]
			+ SGridPanel::Slot(1, 2)
				.HAlign(HAlign_Right)
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					[
						SNew(STextBlock)
						.Text(FText::FromString(FString("Head")))
					]
					+ SOverlay::Slot()
						[
							SNew(SBorder) //add visibility check
							.BorderBackgroundColor(FSlateColor(FLinearColor(1, 0, 0, 1)))
							[
								SNew(SBox)
								.WidthOverride(50)
								.HeightOverride(50)
								[
									SNew(SARInventoryItemWidget)
									.SlotType(EItemSlot::Item_Head)
									.EquipmentSlot(EEquipmentSlot::Item_Head)
									.PlayerController(MyPC)
									.SlotName(FText::FromName("Head"))
								]
							]
						]
				]
			+ SGridPanel::Slot(2, 2)
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					[
						SNew(STextBlock)
						.Text(FText::FromString(FString("Shoulders")))
					]
					+ SOverlay::Slot()
						[
							SNew(SBorder) //add visibility check
							.BorderBackgroundColor(FSlateColor(FLinearColor(1, 0, 0, 1)))
							[
								SNew(SBox)
								.WidthOverride(50)
								.HeightOverride(50)
								[
									SNew(SARInventoryItemWidget)
									.SlotType(EItemSlot::Item_Shoulders)
									.EquipmentSlot(EEquipmentSlot::Item_Shoulders)
									.PlayerController(MyPC)
									.SlotName(FText::FromName("Shoulders"))
								]
							]
						]
				]
			+ SGridPanel::Slot(3, 2)
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					[
						SNew(STextBlock)
						.Text(FText::FromString(FString("Hands")))
					]
					+ SOverlay::Slot()
						[
							SNew(SBorder) //add visibility check
							.BorderBackgroundColor(FSlateColor(FLinearColor(1, 0, 0, 1)))
							[
								SNew(SBox)
								.WidthOverride(50)
								.HeightOverride(50)
								[
									SNew(SARInventoryItemWidget)
									.SlotType(EItemSlot::Item_Hands)
									.EquipmentSlot(EEquipmentSlot::Item_Hands)
									.PlayerController(MyPC)
									.SlotName(FText::FromName("Hands"))
								]
							]
						]
				]
			+ SGridPanel::Slot(4, 2)
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					[
						SNew(STextBlock)
						.Text(FText::FromString(FString("Chest")))
					]
					+ SOverlay::Slot()
						[
							SNew(SBorder) //add visibility check
							.BorderBackgroundColor(FSlateColor(FLinearColor(1, 0, 0, 1)))
							[
								SNew(SBox)
								.WidthOverride(50)
								.HeightOverride(50)
								[
									SNew(SARInventoryItemWidget)
									.SlotType(EItemSlot::Item_Chest)
									.EquipmentSlot(EEquipmentSlot::Item_Chest)
									.PlayerController(MyPC)
									.SlotName(FText::FromName("Chest"))
								]
							]
						]
				]
			+ SGridPanel::Slot(5, 2)
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					[
						SNew(STextBlock)
						.Text(FText::FromString(FString("Legs")))
					]
					+ SOverlay::Slot()
						[
							SNew(SBorder) //add visibility check
							.BorderBackgroundColor(FSlateColor(FLinearColor(1, 0, 0, 1)))
							[
								SNew(SBox)
								.WidthOverride(50)
								.HeightOverride(50)
								[
									SNew(SARInventoryItemWidget)
									.SlotType(EItemSlot::Item_Legs)
									.EquipmentSlot(EEquipmentSlot::Item_Legs)
									.PlayerController(MyPC)
									.SlotName(FText::FromName("Legs"))
								]
							]
						]
				]
			+ SGridPanel::Slot(6, 2)
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					[
						SNew(STextBlock)
						.Text(FText::FromString(FString("Foot")))
					]
					+ SOverlay::Slot()
						[
							SNew(SBorder) //add visibility check
							.BorderBackgroundColor(FSlateColor(FLinearColor(1, 0, 0, 1)))
							[
								SNew(SBox)
								.WidthOverride(50)
								.HeightOverride(50)
								[
									SNew(SARInventoryItemWidget)
									.SlotType(EItemSlot::Item_Foot)
									.EquipmentSlot(EEquipmentSlot::Item_Foot)
									.PlayerController(MyPC)
									.SlotName(FText::FromName("Foot"))
								]
							]
						]
				]

		];
}
