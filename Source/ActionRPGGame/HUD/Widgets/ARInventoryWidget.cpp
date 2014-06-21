// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../../Types/ARStructTypes.h"
#include "../../ARPlayerController.h"
#include "../../ARCharacter.h"
#include "../../Componenets/AREquipmentComponent.h"
#include "../../Types/AREnumTypes.h"
#include "../../Items/ARItem.h"
#include "../../Items/ARItemDataAsset.h"

#include "ARInventoryItemWidget.h"

#include "ARInventoryWidget.h"

void SARInventoryWidget::Construct(const FArguments& InArgs)
{
	PlayerController = InArgs._PlayerController;
	Character = InArgs._Character;

	InventoryColumns = InArgs._InventoryColumns;
	InventoryRows = InArgs._InventoryRows;
	InventoryVisibility = InArgs._InventoryVisibility;

	InventoryVisibility = EVisibility::Visible;

	SyncInventory();
	ChildSlot
		[
			SNew(SBox)
			[
				SAssignNew(TileView, STileView<TSharedPtr<FARItemInfo>>)
				.ListItemsSource(&ClonedInventory)
				.OnGenerateTile(this, &SARInventoryWidget::MakeTileViewWidget)
				.ItemHeight(64)
				.ItemWidth(64)
				.Visibility(this, &SARInventoryWidget::GetInventoryVisibility)
				/*SAssignNew(TileView, STileView<TWeakObjectPtr<UObject>>)
				.ListItemsSource(&ItemsTest)
				.OnGenerateTile(this, &SARInventoryWidget::MakeTileViewWidget)
				.ItemHeight(64)
				.ItemWidth(64)*/
			]
		];
}

void SARInventoryWidget::SetInventoryVisibility(EVisibility VisIn)
{
	InventoryVisibility = VisIn;
}

EVisibility SARInventoryWidget::GetInventoryVisibility() const
{
	AARCharacter* MyChar = Cast<AARCharacter>(PlayerController->GetPawn());

	if (MyChar)
	{
		return MyChar->GetInventoryVisibility();
	}
	return EVisibility::Collapsed;
	//return InventoryVisibility;
}
//TSharedRef<ITableRow> SARInventoryWidget::MakeTileViewWidget(TWeakObjectPtr<class UObject> AssetItem, const TSharedRef<STableViewBase>& OwnerTable)
//{
//
//	if (AssetItem.IsValid())
//	{
//		TSharedRef< STableRow<TWeakObjectPtr<class UObject> >> ReturnRow = SNew(STableRow<TWeakObjectPtr<class UObject>>, OwnerTable)
//			.Content()
//			[
//				SNew(SARInventoryItemWidget)
//				.InventoryItemObj(AssetItem)
//				.PlayerController(PlayerController)
//				.Character(Character)
//				.SlotType(EItemSlot::Item_Inventory)//set inventory slot type to well inventory.
//				.SlotName(FText::FromName("Inv"))
//			];
//		return ReturnRow;
//	}
//	TSharedPtr< STableRow<TSharedPtr<FARItemInfo>> > TableRowWidget;
//	return TableRowWidget.ToSharedRef();
//}
TSharedRef<ITableRow> SARInventoryWidget::MakeTileViewWidget(TSharedPtr<FARItemInfo> AssetItem, const TSharedRef<STableViewBase>& OwnerTable)
{

	if (AssetItem.IsValid())
	{
		TSharedRef< STableRow<TSharedPtr<FARItemInfo> >> ReturnRow = SNew(STableRow<TSharedPtr<FARItemInfo>>, OwnerTable)
			.Content()
			[
				SNew(SARInventoryItemWidget)
				.InventoryItem(AssetItem)
				.PlayerController(PlayerController)
				.Character(Character)
				.SlotType(EItemSlot::Item_Inventory)//set inventory slot type to well inventory.
				.SlotName(FText::FromName("Inv"))
			];
		return ReturnRow;
	}
		TSharedPtr< STableRow<TSharedPtr<FARItemInfo>> > TableRowWidget;
	
		//SAssignNew(TableRowWidget, STableRow<TSharedPtr<FARItemInfo>>, OwnerTable);
	
		//TWeakPtr<STextBlock> item =
		//	SNew(STextBlock)
		//	.Text(FText::FromName(AssetItem->TestName));
	
		//if (item.IsValid())
		//{
		//	TableRowWidget->SetContent(item.Pin().ToSharedRef());
		//}
		//TableRowWidget->SetColorAndOpacity(FLinearColor(1, 0, 0, 1));
		//
		return TableRowWidget.ToSharedRef();
}
void SARInventoryWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
	AARPlayerController* const pPlayerController = PlayerController.Get();
	if (pPlayerController)
	{
		const AARCharacter* const pChar = Cast<AARCharacter>(pPlayerController->GetPawn());
		if (pPlayerController->IsInventoryChanged)
		{
			SyncInventory();
			TileView->RequestListRefresh();
			pPlayerController->IsInventoryChanged = false;
		}
	}
}

void SARInventoryWidget::SyncInventory()
{
	const AARPlayerController* const pPlayerController = PlayerController.Get();
	//const AARCharacter* const pChar = Character.Get();
	//if (!pChar)
	//	return;

	if (pPlayerController)
	{
		ClonedInventory.Empty(pPlayerController->Inventory.Num());

		for (const FARItemInfo& InventoryItem : pPlayerController->Inventory)
		{
			ClonedInventory.Add(MakeShareable(new FARItemInfo(InventoryItem)));
		}
		//ItemDataAssetPath = "/Game/Blueprints/Items_DataAsset.Items_DataAsset";
		//TWeakObjectPtr<UARItemDataAsset> ItemDataAsset = Cast<UARItemDataAsset>(StaticLoadObject(UARItemDataAsset::StaticClass(), NULL, *ChestDataAssetPathAwsome, NULL, LOAD_None, NULL));
		//for (const FInventorySlot& InventoryItem : pPlayerController->InventorySmall)
		//{
		//	if (ItemDataAsset.IsValid() && ItemDataAsset->Items.Num() > 0)
		//	{
		//		FARItemInfo item;
		//		item.ItemName = InventoryItem.ItemName;

		//		int32 index = ItemDataAsset->Items.IndexOfByKey(item);
		//		ClonedInventory.Add(MakeShareable(new FARItemInfo(ItemDataAsset->Items[index])));
		//	}
		//}
	}

	//for (int i = 0; i < 50; i++)
	//{
	//	ClonedInventory.Add(MakeShareable(new FARItemInfo));
	//}
}