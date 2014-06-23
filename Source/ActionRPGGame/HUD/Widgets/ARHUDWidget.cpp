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
							.SlotType(EItemSlot::Item_Weapon)
							.EquipmentSlot(EEquipmentSlot::Item_LeftHandOne)
							.PlayerController(MyPC)
							.SlotName(FText::FromName("LHand"))
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
							.SlotName(FText::FromName("LHand"))
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
							.SlotName(FText::FromName("LHand"))
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
							.SlotName(FText::FromName("LHand"))
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
			+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Top)
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
			+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Top)
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
			+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Top)
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
			+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Top)
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
				.VAlign(VAlign_Top)
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
			+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Top)
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

		];
}
