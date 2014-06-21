// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../../Types/ARStructTypes.h"

#include "ARInventoryWidget.h"
#include "ARInventoryItemWidget.h"

#include "ARHUDWidget.h"


void SARHUDWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;
	MyPC = InArgs._MyPC;

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
			//begin for Character Equipment UI.
			+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Bottom)
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
			+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Bottom)
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
							.SlotName(FText::FromName("LeftHandOne"))
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
						.WidthOverride(50)
						.HeightOverride(50)
						[
							SNew(SARInventoryItemWidget)
							.SlotType(EItemSlot::Item_Weapon)
							.EquipmentSlot(EEquipmentSlot::Item_LeftHandTwo)
							.PlayerController(MyPC)
							.SlotName(FText::FromName("LeftHandTwo"))
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
						.WidthOverride(50)
						.HeightOverride(50)
						[
							SNew(SARInventoryItemWidget)
							.SlotType(EItemSlot::Item_Weapon)
							.EquipmentSlot(EEquipmentSlot::Item_LeftHandThree)
							.PlayerController(MyPC)
							.SlotName(FText::FromName("LeftHandThree"))
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
						.WidthOverride(50)
						.HeightOverride(50)
						[
							SNew(SARInventoryItemWidget)
							.SlotType(EItemSlot::Item_Weapon)
							.EquipmentSlot(EEquipmentSlot::Item_LeftHandFour)
							.PlayerController(MyPC)
							.SlotName(FText::FromName("LeftHandFour"))
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

		];
}
