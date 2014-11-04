// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../ARHUD.h"

#include "../../Types/ARStructTypes.h"

#include "SlateBasics.h"

class SARInventoryWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SARInventoryWidget)
	{}
	/*See private declaration of OwnerHUD below.*/

		SLATE_ARGUMENT(TWeakObjectPtr<class AARPlayerController>, PlayerController)

		SLATE_ARGUMENT(TWeakObjectPtr<class UARInventoryComponent>, Inventory)

		SLATE_ARGUMENT(TWeakObjectPtr<class AARCharacter>, Character)

		SLATE_ARGUMENT(int32, InventoryColumns)

		SLATE_ARGUMENT(int32, InventoryRows)

		SLATE_ARGUMENT(EVisibility, InventoryVisibility)
	SLATE_END_ARGS()
public:
	void Construct(const FArguments& InArgs);
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	void SetInventoryVisibility(EVisibility VisIn);
protected:

private:
	TWeakObjectPtr<class AARPlayerController> PlayerController;
	TWeakObjectPtr<class AARCharacter> Character;
	TWeakObjectPtr<class UARInventoryComponent> Inventory;
	TSharedPtr<SGridPanel> GridPanel;

	TArray < TWeakObjectPtr<class UObject>> ItemsTest;

	TSharedPtr<STileView<TSharedPtr<FARDragDropInfo>>> TileView;
	TSharedRef<ITableRow> MakeTileViewWidget(TSharedPtr<FARDragDropInfo> AssetItem, const TSharedRef<STableViewBase>& OwnerTable);

	TArray<TSharedPtr<FARDragDropInfo>> ClonedInventory;
	//FString ItemDataAssetPath;

	int32 InventoryColumns;
	int32 InventoryRows;
	void SyncInventory();

};



