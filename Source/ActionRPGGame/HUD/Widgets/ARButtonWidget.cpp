// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../../Types/ARStructTypes.h"
#include "../../ARCharacter.h"
#include "../../ARPlayerController.h"
#include "../../Componenets/AREquipmentComponent.h"
#include "../../Types/AREnumTypes.h"

#include "ARInventoryWidget.h"
#include "ARInventoryItemWidget.h"

#include "ARButtonWidget.h"
SARButtonWidget::~SARButtonWidget()
{
	//Items.Empty();
}
void SARButtonWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Top)
			[
				SNew(SBorder)
				.BorderBackgroundColor(FSlateColor(FLinearColor(1,0,0,1)))
				[
					SNew(SBox)
					.WidthOverride(320)
					.HeightOverride(640)
					//.Content(640)
					[
						SNew(SARInventoryWidget)
						.PlayerController(MyPC)
						.Character(Character)
						.InventoryColumns(5)
						.InventoryRows(10)
					]
				]
			]
			+ SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Bottom)
				[
					SNew(SARInventoryItemWidget)
					.SlotType(EItemSlot::Item_Chest)
					.PlayerController(MyPC)
					.SlotName(FText::FromName("Chest"))
				]

			//+ SOverlay::Slot()
			//.VAlign(VAlign_Top)
			//.HAlign(HAlign_Center)
			//[
			//	SNew(STextBlock)
			//	.ShadowColorAndOpacity(FLinearColor::Black)
			//	.ColorAndOpacity(FLinearColor::White)
			//	.ShadowOffset(FIntPoint(-1, 1))
			//	.Font(FSlateFontInfo("Veranda", 16)) //don't believe this works, see Rama's tutorial
			//	.Text(FText::FromString("Hello, Slate!"))
			//]
			//+SOverlay::Slot()
			//.VAlign(VAlign_Center)
			//.HAlign(HAlign_Center)
			//[
			//	SNew(STextBlock)
			//	.ShadowColorAndOpacity(FLinearColor::Black)
			//	.ColorAndOpacity(FLinearColor::White)
			//	.ShadowOffset(FIntPoint(-1, 1))
			//	.Font(FSlateFontInfo("Veranda", 16)) //don't believe this works, see Rama's tutorial
			//	.Text(FText::FromString("Hello, Slate 2!"))
			//]
			//+SOverlay::Slot()
			
			//.VAlign(VAlign_Center)
			//.HAlign(HAlign_Left)
			//[
			//	SNew(SARItemView)
			//	//.SingleItem(GetSingleItem())
			//	.IsEnabled(true)
			//	.OnMouseEnter(this, &SARButtonWidget::DoNothing)
			//	.OnMouseLeave(this, &SARButtonWidget::DoNothing)
			//	//.OnMouseButtonDown(this, &SARButtonWidget::DoNothing)	
			//]
			//+SOverlay::Slot()
			//[
				//SAssignNew(ActionGrid, SGridPanel)
			//	//SNew(STileView<TSharedPtr<FARItemInfo>>)
			//	//.ListItemsSource(&Items)
			//	//.OnGenerateTile(this, &SARButtonWidget::MakeTileViewWidget)
			//	//.ItemHeight(50)
			////	//.ItemWidth(50)
			//	SNew(SARInventoryItem)
			//	.OwnerHUD(OwnerHUD)
			//	.MyPC(MyPC)
			//]
			
		];

	//if (Items.Num() > 0)
	//{
		//for (int32 i = 0; i < 5; i++)
		//{
		//	for (int32 j = 0; j < 10; j++)
		//	{
		//		ActionGrid->AddSlot(i, j)
		//			.HAlign(HAlign_Center)
		//			.VAlign(VAlign_Center)
		//			[
		//				SNew(SARInventoryItem)
		//				.OwnerHUD(OwnerHUD)
		//				.MyPC(MyPC)
		//				.Inventory(&ClonedInventory)
		//				//.OnMouseEnter(this, &SARButtonWidget::DoNothing)
		//				//.OnMouseLeave(this, &SARButtonWidget::DoNothing)
		//			];
		//	}
		//}
	//}

}
void SARButtonWidget::SyncInventory()
{
	//const AARPlayerController* const pPlayerController = MyPC.Get();
	//ClonedInventory.Empty(pPlayerController->Equipment->Inventory.Num());

	//for (const FARItemInfo& InventoryItem : pPlayerController->Equipment->Inventory)
	//{
	//	ClonedInventory.Add(MakeShareable(new FARItemInfo(InventoryItem)));
	//}
}
void SARButtonWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
	SyncInventory();
	//TWeakObjectPtr<AARPlayerController> MyPC = Cast<AARPlayerController>(OwnerHUD->GetOwningPlayerController());
	//if (MyPC.IsValid())
	//{
	//	Items = MyPC->Equipment->Inventory;
	//}
}

TArray<TSharedPtr<FARItemInfo>> SARButtonWidget::GetInventory()
{
	return ClonedInventory;
}

TArray<FARItemInfo> SARButtonWidget::GetItems()
{
	return Items;
}


FReply SARButtonWidget::DoNothing(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled();
}

void SARButtonWidget::EmptyFunction(FGeometry const& MyGeometry, FDragDropEvent const& DragDropEvent)
{
	float a = 10;
	a += 10;
}



//TSharedRef<ITableRow> SARButtonWidget::MakeTileViewWidget(TSharedPtr<FARItemInfo> AssetItem, const TSharedRef<STableViewBase>& OwnerTable)
//{
//
//	if (AssetItem.IsValid())
//	{
//		TSharedRef< STableRow<TSharedPtr<FARItemInfo> >> ReturnRow = SNew(STableRow<TSharedPtr<FARItemInfo>>, OwnerTable)
//			//.OnDragEnter(this, &SARButtonWidget::EmptyFunction)
//			.Content()
//			[
//				//SNew(STextBlock)
//				//.Text(FText::AsNumber(AssetItem->TestIndex))
//				SNew(SARInventoryItem)
//			];
//		return ReturnRow;
//	}
//
//	TSharedPtr< STableRow<TSharedPtr<FARItemInfo>> > TableRowWidget;
//
//	//SAssignNew(TableRowWidget, STableRow<TSharedPtr<FARItemInfo>>, OwnerTable);
//
//	//TWeakPtr<STextBlock> item =
//	//	SNew(STextBlock)
//	//	.Text(FText::FromName(AssetItem->TestName));
//
//	//if (item.IsValid())
//	//{
//	//	TableRowWidget->SetContent(item.Pin().ToSharedRef());
//	//}
//	//TableRowWidget->SetColorAndOpacity(FLinearColor(1, 0, 0, 1));
//	//
//	return TableRowWidget.ToSharedRef();
//}
