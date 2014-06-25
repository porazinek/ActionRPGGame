// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../ARHUD.h"

#include "../../Types/ARStructTypes.h"

#include "Slate.h"

class SARAbilityInventoryWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SARAbilityInventoryWidget)
	{}
	/*See private declaration of OwnerHUD below.*/
		SLATE_ARGUMENT(TWeakObjectPtr<class AARHUD>, OwnerHUD)

		SLATE_ARGUMENT(TWeakObjectPtr<class AARPlayerController>, MyPC)
	SLATE_END_ARGS()

public:
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/////Needed for every widget
	/////Builds this widget and any of it's children
	void Construct(const FArguments& InArgs);
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:
	TWeakObjectPtr<class AARHUD> OwnerHUD;

	TWeakObjectPtr<class AARPlayerController> MyPC;

	void SyncAbilities();

	TArray<TSharedPtr<FAbilityInfo>> Abilities;

	TSharedPtr<STileView<TSharedPtr<FAbilityInfo>>> AbilityTile;
	TSharedRef<ITableRow> MakeTileViewWidget(TSharedPtr<FAbilityInfo> AssetItem, const TSharedRef<STableViewBase>& OwnerTable);

};



