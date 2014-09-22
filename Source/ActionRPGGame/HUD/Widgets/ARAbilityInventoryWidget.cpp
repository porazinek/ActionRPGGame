// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "ActionRPGGame.h"

#include "../../Types/ARStructTypes.h"
#include "../../ARPlayerController.h"

#include "../../Abilities/ARAbilityComponent.h"

#include "ARCharacterSheetWidget.h"
#include "ARActionItemWidget.h"

#include "ARAbilityInventoryWidget.h"


void SARAbilityInventoryWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;
	MyPC = InArgs._MyPC;
	AbilityComp = InArgs._AbilityComp;

	SyncAbilities();

	ChildSlot
		[
			SNew(SBorder)
			[
				SAssignNew(AbilityTile, STileView<TSharedPtr<FActionSlotInfo>>)
				.ListItemsSource(&Abilities)
				.OnGenerateTile(this, &SARAbilityInventoryWidget::MakeTileViewWidget)
				.ItemHeight(50)
				.ItemWidth(50)
			]
		];
}

void SARAbilityInventoryWidget::SyncAbilities()
{
	if (!AbilityComp.IsValid())
		return;

	Abilities.Empty(AbilityComp->AbilityBook.Num());

	for (const FActionSlotInfo& AbilityIn : AbilityComp->AbilityBook)
	{
		Abilities.Add(MakeShareable(new FActionSlotInfo(AbilityIn)));
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

TSharedRef<ITableRow> SARAbilityInventoryWidget::MakeTileViewWidget(TSharedPtr<FActionSlotInfo> AssetItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (AssetItem.IsValid())
	{
		TSharedRef< STableRow<TSharedPtr<FActionSlotInfo> >> ReturnRow = SNew(STableRow<TSharedPtr<FActionSlotInfo>>, OwnerTable)
			.Content()
			[
				SNew(SARActionItemWidget)
				.MyPC(MyPC)
				.AbilityComp(AbilityComp)
				.CurrentAbility(AssetItem)
				.SlotType(EARAbilitySlot::Ability_Inventory)
			];
		return ReturnRow;
	}
	TSharedPtr< STableRow<TSharedPtr<FActionSlotInfo>> > TableRowWidget;
	return TableRowWidget.ToSharedRef();
}
