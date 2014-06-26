// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "ActionRPGGame.h"

#include "../../Types/ARStructTypes.h"
#include "../../ARPlayerController.h"

#include "ARCharacterSheetWidget.h"
#include "ARActionItemWidget.h"

#include "ARAbilityInventoryWidget.h"


void SARAbilityInventoryWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;
	MyPC = InArgs._MyPC;

	SyncAbilities();

	ChildSlot
		[
			SNew(SBorder)
			[
				SAssignNew(AbilityTile, STileView<TSharedPtr<FAbilityInfo>>)
				.ListItemsSource(&Abilities)
				.OnGenerateTile(this, &SARAbilityInventoryWidget::MakeTileViewWidget)
				.ItemHeight(50)
				.ItemWidth(50)
			]
		];
}

void SARAbilityInventoryWidget::SyncAbilities()
{
	if (!MyPC.IsValid())
		return;

	Abilities.Empty(MyPC->AbilityInventory.Num());

	for (const FAbilityInfo& AbilityIn : MyPC->AbilityInventory)
	{
		Abilities.Add(MakeShareable(new FAbilityInfo(AbilityIn)));
	}
}

void SARAbilityInventoryWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	if (!MyPC.IsValid())
		return;

	if (MyPC->UpdateAbilityInventory)
	{
		SyncAbilities();
		AbilityTile->RequestListRefresh();
		MyPC->UpdateAbilityInventory = false;
	}
}

TSharedRef<ITableRow> SARAbilityInventoryWidget::MakeTileViewWidget(TSharedPtr<FAbilityInfo> AssetItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (AssetItem.IsValid())
	{
		TSharedRef< STableRow<TSharedPtr<FAbilityInfo> >> ReturnRow = SNew(STableRow<TSharedPtr<FAbilityInfo>>, OwnerTable)
			.Content()
			[
				SNew(SARActionItemWidget)
				.MyPC(MyPC)
				.CurrentAbility(AssetItem)
				.SlotType(EARAbilitySlot::Ability_Inventory)
			];
		return ReturnRow;
	}
	TSharedPtr< STableRow<TSharedPtr<FAbilityInfo>> > TableRowWidget;
	return TableRowWidget.ToSharedRef();
}
