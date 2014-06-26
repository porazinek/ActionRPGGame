// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../ARHUD.h"

#include "../../Types/ARStructTypes.h"

#include "Slate.h"

class SARCharacterSheetWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SARCharacterSheetWidget)
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

	TSharedPtr<STileView<TSharedPtr<FInventorySlot>>> LeftWeapon;
	TSharedRef<ITableRow> MakeLeftHandWeaponWidget(TSharedPtr<FInventorySlot> AssetItem, const TSharedRef<STableViewBase>& OwnerTable);
	TArray<TSharedPtr<FInventorySlot>> LeftHandWeapons;
	void SyncLeftHandWeapons();

	TSharedPtr<STileView<TSharedPtr<FInventorySlot>>> RightWeapon;
	TSharedRef<ITableRow> MakeRightHandWeaponWidget(TSharedPtr<FInventorySlot> AssetItem, const TSharedRef<STableViewBase>& OwnerTable);
	TArray<TSharedPtr<FInventorySlot>> RightHandWeapons;
	void SyncRightHandWeapons();
	TSharedPtr<STileView<TSharedPtr<FInventorySlot>>> EquipedItems;
};



