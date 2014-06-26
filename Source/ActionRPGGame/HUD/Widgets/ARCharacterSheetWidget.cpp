// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../../Types/ARStructTypes.h"
#include "../../ARPlayerController.h"

#include "ARInventoryItemWidget.h"

#include "ARCharacterSheetWidget.h"


void SARCharacterSheetWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;
	MyPC = InArgs._MyPC;
	
	SyncLeftHandWeapons();
	SyncRightHandWeapons();
	ChildSlot
		[
			SNew(SGridPanel)
			+ SGridPanel::Slot(1, 1)
			[
				SNew(SBorder) //add visibility check
				.BorderBackgroundColor(FSlateColor(FLinearColor(1, 0, 0, 1)))
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
						[
							SNew(SBox)
							.HeightOverride(56)
							.WidthOverride(250)
							[
								SAssignNew(LeftWeapon, STileView<TSharedPtr<FInventorySlot>>)
								.ListItemsSource(&LeftHandWeapons)
								.OnGenerateTile(this, &SARCharacterSheetWidget::MakeLeftHandWeaponWidget)
								.ItemHeight(50)
								.ItemWidth(50)
							]
						]
				]
			]
				+ SGridPanel::Slot(1, 2)
				[
					SNew(SBorder) //add visibility check
					.BorderBackgroundColor(FSlateColor(FLinearColor(1, 0, 0, 1)))
					[
						SNew(SOverlay)
						+ SOverlay::Slot()
						[
							SNew(SBox)
							.HeightOverride(56)
							.WidthOverride(250)
							[
								SAssignNew(RightWeapon, STileView<TSharedPtr<FInventorySlot>>)
								.ListItemsSource(&RightHandWeapons)
								.OnGenerateTile(this, &SARCharacterSheetWidget::MakeRightHandWeaponWidget)
								.ItemHeight(50)
								.ItemWidth(50)
							]
						]
					]
				]
			//+ SGridPanel::Slot(1, 1)
			//.HAlign(HAlign_Right)
			//[
			//	SNew(SBorder) //add visibility check
			//	.BorderBackgroundColor(FSlateColor(FLinearColor(1, 0, 0, 1)))
			//	[
			//		SNew(SOverlay)
			//		+ SOverlay::Slot()
			//		[
			//			SNew(STextBlock)
			//			.Text(FText::FromString(FString("LHand1")))
			//		]
			//		+ SOverlay::Slot()
			//			[
			//				SNew(SBox)
			//				.WidthOverride(50)
			//				.HeightOverride(50)
			//				[
			//					SNew(SARInventoryItemWidget)
			//					.SlotType(EItemSlot::Item_Weapon)
			//					.EquipmentSlot(EEquipmentSlot::Item_LeftHandOne)
			//					.PlayerController(MyPC)
			//					.SlotName(FText::FromName("LHand"))
			//				]
			//			]
			//	]
			//]
			//+ SGridPanel::Slot(2, 1)
			//	.HAlign(HAlign_Right)
			//	[
			//		SNew(SOverlay)
			//		+ SOverlay::Slot()
			//		[
			//			SNew(STextBlock)
			//			.Text(FText::FromString(FString("LHand2")))
			//		]
			//		+ SOverlay::Slot()
			//			[
			//				SNew(SBorder) //add visibility check
			//				.BorderBackgroundColor(FSlateColor(FLinearColor(1, 0, 0, 1)))
			//				[
			//					SNew(SBox)
			//					.WidthOverride(50)
			//					.HeightOverride(50)
			//					[
			//						SNew(SARInventoryItemWidget)
			//						.SlotType(EItemSlot::Item_Weapon)
			//						.EquipmentSlot(EEquipmentSlot::Item_LeftHandOne)
			//						.PlayerController(MyPC)
			//						.SlotName(FText::FromName("LHand"))
			//					]
			//				]
			//			]
			//	]
			//+ SGridPanel::Slot(3, 1)
			//	.HAlign(HAlign_Right)
			//	[
			//		SNew(SOverlay)
			//		+ SOverlay::Slot()
			//		[
			//			SNew(STextBlock)
			//			.Text(FText::FromString(FString("LHand3")))
			//		]
			//		+ SOverlay::Slot()
			//			[
			//				SNew(SBorder) //add visibility check
			//				.BorderBackgroundColor(FSlateColor(FLinearColor(1, 0, 0, 1)))
			//				[
			//					SNew(SBox)
			//					.WidthOverride(50)
			//					.HeightOverride(50)
			//					[
			//						SNew(SARInventoryItemWidget)
			//						.SlotType(EItemSlot::Item_Weapon)
			//						.EquipmentSlot(EEquipmentSlot::Item_LeftHandOne)
			//						.PlayerController(MyPC)
			//						.SlotName(FText::FromName("LHand"))
			//					]
			//				]
			//			]
			//	]
			//+ SGridPanel::Slot(4, 1)
			//	.HAlign(HAlign_Right)
			//	[
			//		SNew(SOverlay)
			//		+ SOverlay::Slot()
			//		[
			//			SNew(STextBlock)
			//			.Text(FText::FromString(FString("LHand4")))
			//		]
			//		+ SOverlay::Slot()
			//			[
			//				SNew(SBorder) //add visibility check
			//				.BorderBackgroundColor(FSlateColor(FLinearColor(1, 0, 0, 1)))
			//				[
			//					SNew(SBox)
			//					.WidthOverride(50)
			//					.HeightOverride(50)
			//					[
			//						SNew(SARInventoryItemWidget)
			//						.SlotType(EItemSlot::Item_Weapon)
			//						.EquipmentSlot(EEquipmentSlot::Item_LeftHandOne)
			//						.PlayerController(MyPC)
			//						.SlotName(FText::FromName("LHand"))
			//					]
			//				]
			//			]
			//	]
			+ SGridPanel::Slot(1, 3)
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
			+ SGridPanel::Slot(2, 3)
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
			+ SGridPanel::Slot(3, 3)
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
			+ SGridPanel::Slot(4, 3)
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
			+ SGridPanel::Slot(5, 3)
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
			+ SGridPanel::Slot(6, 3)
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

void SARCharacterSheetWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
	if (MyPC->LeftHandWeaponsUpdated)
	{
		SyncLeftHandWeapons();
		LeftWeapon->RequestListRefresh();
		MyPC->LeftHandWeaponsUpdated = false;
	}
	if (MyPC->RightHandWeaponsUpdated)
	{
		SyncRightHandWeapons();
		RightWeapon->RequestListRefresh();
		MyPC->RightHandWeaponsUpdated = false;
	}
}

TSharedRef<ITableRow> SARCharacterSheetWidget::MakeLeftHandWeaponWidget(TSharedPtr<FInventorySlot> AssetItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (AssetItem.IsValid())
	{
		TSharedRef< STableRow<TWeakObjectPtr<class UObject> >> ReturnRow = SNew(STableRow<TWeakObjectPtr<class UObject>>, OwnerTable)
			.Content()
			[
				SNew(SARInventoryItemWidget)
				.InventoryItemObj(AssetItem)
				.PlayerController(MyPC)
				.SlotType(EItemSlot::Item_Weapon)//set inventory slot type to well inventory.
				.EquipmentSlot(EEquipmentSlot::Item_LeftHandOne)
				.SlotName(FText::FromName("LHand"))
			];
		return ReturnRow;
	}
	TSharedPtr< STableRow<TSharedPtr<FARItemInfo>> > TableRowWidget;
	return TableRowWidget.ToSharedRef();
}
void SARCharacterSheetWidget::SyncLeftHandWeapons()
{
	if (MyPC.IsValid())
	{
		LeftHandWeapons.Empty(MyPC->LeftHandWeapons.Num());
		for (const FInventorySlot& InventoryItem : MyPC->LeftHandWeapons)
		{
			LeftHandWeapons.Add(MakeShareable(new FInventorySlot(InventoryItem)));
		}
	}
}


TSharedRef<ITableRow> SARCharacterSheetWidget::MakeRightHandWeaponWidget(TSharedPtr<FInventorySlot> AssetItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (AssetItem.IsValid())
	{
		TSharedRef< STableRow<TWeakObjectPtr<class UObject> >> ReturnRow = SNew(STableRow<TWeakObjectPtr<class UObject>>, OwnerTable)
			.Content()
			[
				SNew(SARInventoryItemWidget)
				.InventoryItemObj(AssetItem)
				.PlayerController(MyPC)
				.SlotType(EItemSlot::Item_Weapon)//set inventory slot type to well inventory.
				.EquipmentSlot(EEquipmentSlot::Item_RightHandOne)
				.SlotName(FText::FromName("RHand"))
			];
		return ReturnRow;
	}
	TSharedPtr< STableRow<TSharedPtr<FARItemInfo>> > TableRowWidget;
	return TableRowWidget.ToSharedRef();
}
void SARCharacterSheetWidget::SyncRightHandWeapons()
{
	if (MyPC.IsValid())
	{
		RightHandWeapons.Empty(MyPC->RightHandWeapons.Num());
		for (const FInventorySlot& InventoryItem : MyPC->RightHandWeapons)
		{
			RightHandWeapons.Add(MakeShareable(new FInventorySlot(InventoryItem)));
		}
	}
}