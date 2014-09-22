// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "ActionRPGGame.h"

#include "../../Types/ARStructTypes.h"
#include "../../ARPlayerController.h"
#include "../../ARCharacter.h"
#include "../../Abilities/ARAbilityComponent.h"

#include "ARActionItemWidget.h"

#include "ARActionBarWidget.h"


void SARActionBarWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;
	MyPC = InArgs._MyPC;

	AbilityComp = InArgs._AbilityComp;
	AbilityComp->OnActionAddedToBar.BindRaw(this, &SARActionBarWidget::OnRefreshBar);
	SyncAbilities();

	ChildSlot
		[
			SNew(SBorder)
			[
				SAssignNew(AbilityTile, STileView<TSharedPtr<FActionSlotInfo>>)
				.ListItemsSource(&Abilities)
				.OnGenerateTile(this, &SARActionBarWidget::MakeTileViewWidget)
				.ItemHeight(50)
				.ItemWidth(50)
			]
		];
}
void SARActionBarWidget::OnRefreshBar(FActionSlotInfo ActionSlot)
{
	if (!AbilityComp.IsValid())
		return;

	Abilities.Empty(AbilityComp->ActionBars.ActionBars[0].ActionSlots.Num());
	for (const FActionSlotInfo& AbilityIn : AbilityComp->ActionBars.ActionBars[0].ActionSlots)
	{
		Abilities.Add(MakeShareable(new FActionSlotInfo(AbilityIn)));
	}
	AbilityTile->RequestListRefresh();
}
void SARActionBarWidget::SyncAbilities()
{
	if (!AbilityComp.IsValid())
		return;

	Abilities.Empty(AbilityComp->ActionBars.ActionBars[0].ActionSlots.Num());
	for (const FActionSlotInfo& AbilityIn : AbilityComp->ActionBars.ActionBars[0].ActionSlots)
	{
		Abilities.Add(MakeShareable(new FActionSlotInfo(AbilityIn)));
	}
}

void SARActionBarWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	if (!MyPC.IsValid())
		return;

	if (MyPC->UpdateActionBarOne)
	{
		SyncAbilities();
		AbilityTile->RequestListRefresh();
		MyPC->UpdateActionBarOne = false;
	}
}

TSharedRef<ITableRow> SARActionBarWidget::MakeTileViewWidget(TSharedPtr<FActionSlotInfo> AssetItem, const TSharedRef<STableViewBase>& OwnerTable)
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
				.SlotType(EARAbilitySlot::Ability_ActionBar)
			];
		return ReturnRow;
	}
	TSharedPtr< STableRow<TSharedPtr<FActionSlotInfo>> > TableRowWidget;
	return TableRowWidget.ToSharedRef();
}
