// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../ARHUD.h"

#include "../../Types/ARStructTypes.h"

#include "Slate.h"
/*
	Display this widget only when looting actor.
	Display list of lootable items from actor.
*/
class SARLootingWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SARLootingWidget)
	{}
	/*See private declaration of OwnerHUD below.*/
		SLATE_ARGUMENT(TWeakObjectPtr<class AARHUD>, OwnerHUD)

		SLATE_ARGUMENT(TWeakObjectPtr<class AARPlayerController>, MyPC)

		SLATE_ARGUMENT(TWeakObjectPtr<class UARInventoryComponent>, Inventory)

		SLATE_ARGUMENT(TWeakObjectPtr<class UAREquipmentComponent>, Equipment)

		SLATE_ARGUMENT(TWeakObjectPtr<class AARCharacter>, Character)
	SLATE_END_ARGS()

public:
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/////Needed for every widget
	/////Builds this widget and any of it's children
	void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	TArray<TSharedPtr<FARDragDropInfo>> ClonedLoot;

	TSharedPtr<STileView<TSharedPtr<FARDragDropInfo>>> LootWindowView;

	TSharedRef<ITableRow> MakeTileViewWidget(TSharedPtr<FARDragDropInfo> AssetItem, const TSharedRef<STableViewBase>& OwnerTable);
	
	void CloneLootList(class AARItemPickup* PickedItem);
private:
	TWeakObjectPtr<class AARHUD> OwnerHUD;

	TWeakObjectPtr<class AARPlayerController> MyPC;

	TWeakObjectPtr<class UARInventoryComponent> Inventory;

	TWeakObjectPtr<class UAREquipmentComponent> Equipment;

	TWeakObjectPtr<class AARCharacter> Character;

	TWeakObjectPtr<class AARItemPickup> LootedObject;
};



