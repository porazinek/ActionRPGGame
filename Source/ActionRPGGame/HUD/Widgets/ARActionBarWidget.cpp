// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "ActionRPGGame.h"

#include "../../Types/ARStructTypes.h"
#include "../../ARPlayerController.h"
#include "../../ARCharacter.h"
#include "../../Componenets/ARAbilityComponent.h"

#include "ARActionItemWidget.h"

#include "ARActionBarWidget.h"


void SARActionBarWidget::Construct(const FArguments& InArgs)
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
				.OnGenerateTile(this, &SARActionBarWidget::MakeTileViewWidget)
			]
		];
}
void SARActionBarWidget::SyncAbilities()
{
	if (!MyPC.IsValid())
		return;

	Abilities.Empty(MyPC->ActionBarOne.Num());
	for (const FAbilityInfo& AbilityIn : MyPC->ActionBarOne)
	{
		Abilities.Add(MakeShareable(new FAbilityInfo(AbilityIn)));
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
		MyPC->UpdateActionBarOne = false;
	}
}

TSharedRef<ITableRow> SARActionBarWidget::MakeTileViewWidget(TSharedPtr<FAbilityInfo> AssetItem, const TSharedRef<STableViewBase>& OwnerTable)
{

	if (AssetItem.IsValid())
	{
		TSharedRef< STableRow<TSharedPtr<FAbilityInfo> >> ReturnRow = SNew(STableRow<TSharedPtr<FAbilityInfo>>, OwnerTable)
			.Content()
			[
				SNew(SARActionItemWidget)
			];
		return ReturnRow;
	}
	TSharedPtr< STableRow<TSharedPtr<FAbilityInfo>> > TableRowWidget;
	return TableRowWidget.ToSharedRef();
}
