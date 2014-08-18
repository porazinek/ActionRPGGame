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

		SLATE_ARGUMENT(TWeakObjectPtr<class AARCharacter>, Character)

		SLATE_ARGUMENT(TWeakObjectPtr<class UAREquipmentComponent>, Equipment)
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

	TWeakObjectPtr<class AARCharacter> Character;

	TWeakObjectPtr<class UAREquipmentComponent> Equipment;

	TSharedPtr<STileView<TSharedPtr<FARDragDropInfo>>> EquipmentSlot;
	TSharedRef<ITableRow> MakeEquipmentSlotsWeaponWidget(TSharedPtr<FARDragDropInfo> AssetItem, const TSharedRef<STableViewBase>& OwnerTable);
	TArray<TSharedPtr<FARDragDropInfo>> EquipmentSlots;
	void SyncEquipmentWeapons();

	TSharedPtr<STileView<TSharedPtr<FARDragDropInfo>>> LeftWeapon;
	TSharedRef<ITableRow> MakeLeftHandWeaponWidget(TSharedPtr<FARDragDropInfo> AssetItem, const TSharedRef<STableViewBase>& OwnerTable);
	TArray<TSharedPtr<FARDragDropInfo>> LeftHandWeapons;
	void SyncLeftHandWeapons();

	TSharedPtr<STileView<TSharedPtr<FARDragDropInfo>>> RightWeapon;
	TSharedRef<ITableRow> MakeRightHandWeaponWidget(TSharedPtr<FARDragDropInfo> AssetItem, const TSharedRef<STableViewBase>& OwnerTable);
	TArray<TSharedPtr<FARDragDropInfo>> RightHandWeapons;
	void SyncRightHandWeapons();
	TSharedPtr<STileView<TSharedPtr<FInventorySlot>>> EquipedItems;
};



