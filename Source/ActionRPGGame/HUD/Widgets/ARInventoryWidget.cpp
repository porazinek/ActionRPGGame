// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../../Types/ARStructTypes.h"
#include "../../ARPlayerController.h"
#include "../../ARCharacter.h"
#include "../../Componenets/AREquipmentComponent.h"
#include "../../Types/AREnumTypes.h"
#include "../../Items/ARItem.h"
#include "../../Items/ARItemDataAsset.h"

#include "../../Items/ARInventoryComponent.h"

#include "ARInventoryItemWidget.h"

#include "ARInventoryWidget.h"

void SARInventoryWidget::Construct(const FArguments& InArgs)
{
	PlayerController = InArgs._PlayerController;
	Character = InArgs._Character;
	Inventory = InArgs._Inventory;
	InventoryColumns = InArgs._InventoryColumns;
	InventoryRows = InArgs._InventoryRows;
	SyncInventory();
	ChildSlot
		[
			SNew(SBox)
			[
				SAssignNew(TileView, STileView<TSharedPtr<FInventorySlot>>)
				.ListItemsSource(&ClonedInventory)
				.OnGenerateTile(this, &SARInventoryWidget::MakeTileViewWidget)
				.ItemHeight(64)
				.ItemWidth(64)
			]
		];
}


TSharedRef<ITableRow> SARInventoryWidget::MakeTileViewWidget(TSharedPtr<FInventorySlot> AssetItem, const TSharedRef<STableViewBase>& OwnerTable)
{

	if (AssetItem.IsValid())
	{
		TSharedRef< STableRow<TWeakObjectPtr<class UObject> >> ReturnRow = SNew(STableRow<TWeakObjectPtr<class UObject>>, OwnerTable)
			.Content()
			[
				SNew(SARInventoryItemWidget)
				.InventoryItem(AssetItem)
				.PlayerController(PlayerController)
				.Character(Character)
				.SlotType(EItemSlot::Item_Inventory)//set inventory slot type to well inventory.
				.EquipmentSlot(EEquipmentSlot::Item_Inventory)
				.SlotName(FText::FromName("Inv"))
				.Inventory(Inventory)
				.Equipment(Character->Equipment.Get())
			];
		return ReturnRow;
	}
	TSharedPtr< STableRow<TSharedPtr<FARItemInfo>> > TableRowWidget;
	return TableRowWidget.ToSharedRef();
}


void SARInventoryWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
	if (Inventory.IsValid())
	{
		//there is issue with replication and inventory updates!
		if (Inventory->IsInventoryChanged)
		{
			SyncInventory();
			TileView->RequestListRefresh();
			Inventory->IsInventoryChanged = false;
		}
	}
	//AARPlayerController* const pPlayerController = PlayerController.Get();
	//if (pPlayerController)
	//{
	//	const AARCharacter* const pChar = Cast<AARCharacter>(pPlayerController->GetPawn());
	//	//there is issue with replication and inventory updates!
	//	if (pPlayerController->IsInventoryChanged)
	//	{
	//		SyncInventory();
	//		TileView->RequestListRefresh();
	//		pPlayerController->IsInventoryChanged = false;
	//	}
	//}
}

void SARInventoryWidget::SyncInventory()
{
	if (Inventory.IsValid())
	{
		//ClonedInventory.Empty(pPlayerController->Inventory.Num());
		ClonedInventory.Empty(Inventory->Inventory.Num());
		for (const FInventorySlot& InventoryItem : Inventory->Inventory)
		{
			ClonedInventory.Add(MakeShareable(new FInventorySlot(InventoryItem)));
		}
	}
}