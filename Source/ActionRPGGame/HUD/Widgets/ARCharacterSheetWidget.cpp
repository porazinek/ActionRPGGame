// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../../Types/ARStructTypes.h"
#include "../../ARPlayerController.h"
#include "../../ARCharacter.h"

#include "../../Componenets/AREquipmentComponent.h"

#include "ARInventoryItemWidget.h"

#include "ARCharacterSheetWidget.h"


void SARCharacterSheetWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;
	MyPC = InArgs._MyPC;
	Character = InArgs._Character;
	Equipment = InArgs._Equipment;
	SyncLeftHandWeapons();
	SyncRightHandWeapons();
	SyncEquipmentWeapons();
	ChildSlot
		[
			SNew(SGridPanel)
			+ SGridPanel::Slot(1, 1)
			.ColumnSpan(6)
			[
				SNew(SBorder) //add visibility check
				.BorderBackgroundColor(FSlateColor(FLinearColor(1, 0, 0, 1)))
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					[
						SNew(SBox)
						.HeightOverride(52)
						.WidthOverride(400)
						[
							SAssignNew(LeftWeapon, STileView<TSharedPtr<FARDragDropInfo>>)
							.ListItemsSource(&LeftHandWeapons)
							.OnGenerateTile(this, &SARCharacterSheetWidget::MakeLeftHandWeaponWidget)
							.ItemHeight(50)
							.ItemWidth(50)
						]
					]
				]
			]
			+ SGridPanel::Slot(1, 2)
			.ColumnSpan(6)
			[
				SNew(SBorder) //add visibility check
				.BorderBackgroundColor(FSlateColor(FLinearColor(1, 0, 0, 1)))
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					[
						SNew(SBox)
						.HeightOverride(52)
						.WidthOverride(400)
						[
							SAssignNew(RightWeapon, STileView<TSharedPtr<FARDragDropInfo>>)
							.ListItemsSource(&RightHandWeapons)
							.OnGenerateTile(this, &SARCharacterSheetWidget::MakeRightHandWeaponWidget)
							.ItemHeight(50)
							.ItemWidth(50)
						]
					]
				]
			]
			+ SGridPanel::Slot(1, 3)
			.ColumnSpan(6)
			[
				SNew(SBorder) //add visibility check
				.BorderBackgroundColor(FSlateColor(FLinearColor(1, 0, 0, 1)))
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					[
						SNew(SBox)
						.HeightOverride(52)
						.WidthOverride(400)
						[
							SAssignNew(EquipmentSlot, STileView<TSharedPtr<FARDragDropInfo>>)
							.ListItemsSource(&EquipmentSlots)
							.OnGenerateTile(this, &SARCharacterSheetWidget::MakeEquipmentSlotsWeaponWidget)
							.ItemHeight(50)
							.ItemWidth(50)
						]
					]
				]
			]

		];
}

void SARCharacterSheetWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
	if (Equipment->LeftHandWeaponsUpdated)
	{
		SyncLeftHandWeapons();
		LeftWeapon->RequestListRefresh();
		Equipment->LeftHandWeaponsUpdated = false;
	}
	if (Equipment->RightHandWeaponsUpdated)
	{
		SyncRightHandWeapons();
		RightWeapon->RequestListRefresh();
		Equipment->RightHandWeaponsUpdated = false;
	}
	if (false)
	{
		EquipmentSlot->RequestListRefresh();
		SyncEquipmentWeapons();
	}
}

TSharedRef<ITableRow> SARCharacterSheetWidget::MakeLeftHandWeaponWidget(TSharedPtr<FARDragDropInfo> AssetItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (AssetItem.IsValid())
	{
		TSharedRef< STableRow<TWeakObjectPtr<class UObject> >> ReturnRow = SNew(STableRow<TWeakObjectPtr<class UObject>>, OwnerTable)
			.Content()
			[
				SNew(SARInventoryItemWidget)
				.InventoryItem(AssetItem)
				.PlayerController(MyPC)
				.SlotType(EItemSlot::Item_Weapon)//set inventory slot type to well inventory.
				//		.EquipmentSlot(EEquipmentSlot::Item_LeftHandOne)
				.SlotName(FText::FromName("LHand"))
				.Inventory(MyPC->Inventory)
				.Equipment(Equipment)
			];
		return ReturnRow;
	}
	TSharedPtr< STableRow<TSharedPtr<FARDragDropInfo>> > TableRowWidget;
	return TableRowWidget.ToSharedRef();
}
void SARCharacterSheetWidget::SyncLeftHandWeapons()
{
	if (Equipment.IsValid())
	{
		LeftHandWeapons.Empty(Equipment->LeftHandWeapons.Num());
		for (const FARDragDropInfo& InventoryItem : Equipment->LeftHandWeapons)
		{
			LeftHandWeapons.Add(MakeShareable(new FARDragDropInfo(InventoryItem)));
		}
	}
}


TSharedRef<ITableRow> SARCharacterSheetWidget::MakeRightHandWeaponWidget(TSharedPtr<FARDragDropInfo> AssetItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (AssetItem.IsValid())
	{
		TSharedRef< STableRow<TWeakObjectPtr<class UObject> >> ReturnRow = SNew(STableRow<TWeakObjectPtr<class UObject>>, OwnerTable)
			.Content()
			[
				SNew(SARInventoryItemWidget)
				.InventoryItem(AssetItem)
				.PlayerController(MyPC)
				.SlotType(EItemSlot::Item_Weapon)//set inventory slot type to well inventory.
				//	.EquipmentSlot(EEquipmentSlot::Item_RightHandOne)
				.SlotName(FText::FromName("RHand"))
				.Inventory(MyPC->Inventory)
				.Equipment(Equipment)
			];
		return ReturnRow;
	}
	TSharedPtr< STableRow<TSharedPtr<FARDragDropInfo>> > TableRowWidget;
	return TableRowWidget.ToSharedRef();
}
void SARCharacterSheetWidget::SyncRightHandWeapons()
{
	if (Equipment.IsValid())
	{
		RightHandWeapons.Empty(Equipment->RightHandWeapons.Num());
		for (const FARDragDropInfo& InventoryItem : Equipment->RightHandWeapons)
		{
			RightHandWeapons.Add(MakeShareable(new FARDragDropInfo(InventoryItem)));
		}
	}
}

TSharedRef<ITableRow> SARCharacterSheetWidget::MakeEquipmentSlotsWeaponWidget(TSharedPtr<FARDragDropInfo> AssetItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (AssetItem.IsValid())
	{
		TSharedRef< STableRow<TWeakObjectPtr<class UObject> >> ReturnRow = SNew(STableRow<TWeakObjectPtr<class UObject>>, OwnerTable)
			.Content()
			[
				SNew(SARInventoryItemWidget)
				.InventoryItem(AssetItem)
				.PlayerController(MyPC)
				.SlotType(EItemSlot::Item_Weapon)//set inventory slot type to well inventory.
				//	.EquipmentSlot(EEquipmentSlot::Item_RightHandOne)
				.SlotName(FText::FromName(AssetItem->SlotName))
				.Inventory(MyPC->Inventory)
				.Equipment(Equipment)
			];
		return ReturnRow;
	}
	TSharedPtr< STableRow<TSharedPtr<FARDragDropInfo>> > TableRowWidget;
	return TableRowWidget.ToSharedRef();
}
void SARCharacterSheetWidget::SyncEquipmentWeapons()
{
	if (Equipment.IsValid())
	{
		EquipmentSlots.Empty(Equipment->EquipmentSlots.Num());
		for (const FARDragDropInfo& InventoryItem : Equipment->EquipmentSlots)
		{
			EquipmentSlots.Add(MakeShareable(new FARDragDropInfo(InventoryItem)));
		}
	}
}