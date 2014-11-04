// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "ActionRPGGame.h"

#include "../../Types/ARStructTypes.h"
#include "../../ARPlayerController.h"
#include "../../Types/ARItemTypes.h"
#include "../../Items/ARItemPickup.h"

#include "ARLootingWidget.h"
#include "ARInventoryItemWidget.h"

void SARLootingWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;
	MyPC = InArgs._MyPC;
	Character = InArgs._Character;
	Equipment = InArgs._Equipment;
	Character = InArgs._Character;

	MyPC->OnItemPicked.BindRaw(this, &SARLootingWidget::CloneLootList);
	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		[
			SAssignNew(LootWindowView, STileView<TSharedPtr<FARDragDropInfo>>)
			.ListItemsSource(&ClonedLoot)
			.OnGenerateTile(this, &SARLootingWidget::MakeTileViewWidget)
			.ItemHeight(40)
			.ItemWidth(40)
		]
	];
}

void SARLootingWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
	//CloneLootList(LootedObject.Get());
}
void SARLootingWidget::CloneLootList(class AARItemPickup* PickedItem)
{
	if (PickedItem)
	{
		LootedObject = PickedItem;
		if (PickedItem->LootableItems.Num() > 0)
		{
			ClonedLoot.Empty(ClonedLoot.Num());
			for (const FARDragDropInfo& item : PickedItem->LootableItems)
			{
				ClonedLoot.Add(MakeShareable(new FARDragDropInfo(item)));
			}
		}
	}
	if (LootWindowView.IsValid())
		LootWindowView->RequestListRefresh();
}

TSharedRef<ITableRow> SARLootingWidget::MakeTileViewWidget(TSharedPtr<FARDragDropInfo> AssetItem, const TSharedRef<STableViewBase>& OwnerTable)
{

	if (AssetItem.IsValid())
	{
		TSharedRef< STableRow<TWeakObjectPtr<class UObject> >> ReturnRow = SNew(STableRow<TWeakObjectPtr<class UObject>>, OwnerTable)
			.Content()
			[
				SNew(SARInventoryItemWidget)
				.InventoryItem(AssetItem)
				.PlayerController(MyPC)
				.Character(Character)
				.SlotType(EItemSlot::Item_Inventory)//set inventory slot type to well inventory.
				//.EquipmentSlot(EEquipmentSlot::Item_LootWindow)
				.SlotName(FText::FromName("Loot"))
				.Inventory(Inventory)
				.Equipment(Character->Equipment)
				.LootedObject(LootedObject)
			];
		return ReturnRow;
	}
	TSharedPtr< STableRow<TSharedPtr<FARItemInfo>> > TableRowWidget;
	return TableRowWidget.ToSharedRef();
}