// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../../Types/ARStructTypes.h"

#include "../../ARPlayerController.h"
#include "../../Componenets/AREquipmentComponent.h"

#include "ARInventoryItem.h"

void SARInventoryItem::Construct(const FArguments& InArgs)
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/////Get handle on game resources from GameModule
	//MyUIResources = FModuleManager::GetModuleChecked<ActionRPGGame>(FName("ActionRPGGame")).GetSlateGameResources();
	MyUIResources = FSlateGameResources::New("Batons", "/Game/UI", "/Game/UI");
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/////Get handle on spell_heal Slate Brush
	/////name: member_icon
	Icon = MyUIResources->GetBrush(FName("Inactive_Brush"));
	ImageColor = FSlateColor(FLinearColor(0, 1, 0, 1));
	OwnerHUD = InArgs._OwnerHUD;
	MyPC = InArgs._MyPC;
	Inventory = InArgs._Inventory;

	TextTest = FText::FromName("Leave");
	//SyncInventory();

	ChildSlot
		.VAlign(VAlign_Bottom)
		.HAlign(HAlign_Fill)
		[
			SNew(SOverlay)
			//.VAlign(VAlign_Bottom)
			//.HAlign(HAlign_Fill)
			//.Padding(3)
			//.WidthOverride(50)
			//.HeightOverride(50)
			+SOverlay::Slot()
			[
				SAssignNew(ActionGrid, SGridPanel)
				//SNew(SImage)
				//.Image(this, &SARInventoryItem::GetImage)
				//.ColorAndOpacity(this, &SARInventoryItem::GetColor)
				//SNew(STextBlock)
				//.Text(this, &SARInventoryItem::GetText)
				
				//.ColorAndOpacity(FSlateColor(FLinearColor(0, 1, 0, 1)))
				//.Image()
				//SAssignNew(TileView, STileView<TSharedPtr<FARItemInfo>>)
				//.ListItemsSource(&ClonedInventory)
				//.OnGenerateTile(this, &SARInventoryItem::MakeTileViewWidget)
				//.ListItemSource(&ClonedInventory)
			]

		];


	if (Inventory.Num() > 0)
	{
		for (int32 i = 0; i < 5; i++)
		{
			for (int32 j = 0; j < 10; j++)
			{
				ActionGrid->AddSlot(i, j)
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Text(FText::FromName("Test"))
						.Font(FSlateFontInfo("Veranda", 16))
						//SNew(SARInventoryItem)
						//.OwnerHUD(OwnerHUD)
						//.MyPC(MyPC)
						//.Inventory(&ClonedInventory)
						//.OnMouseEnter(this, &SARButtonWidget::DoNothing)
						//.OnMouseLeave(this, &SARButtonWidget::DoNothing)
					];
			}
		}
	}
}
FText SARInventoryItem::GetText() const
{
	return TextTest;
}
void SARInventoryItem::OnDragEnter(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	float dragTest = 2;
	dragTest += 10;
}
void SARInventoryItem::OnDragLeave(const FDragDropEvent& DragDropEvent)
{
	float DragTest = 3;
	DragTest += 10;
}
FReply SARInventoryItem::OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	return FReply::Handled();
}
FReply SARInventoryItem::OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	return FReply::Handled();
}
void SARInventoryItem::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	TextTest = FText::FromName("Enter");
	Icon = MyUIResources->GetBrush(FName("Active_Brush"));
	ImageColor = FSlateColor(FLinearColor(1, 0, 0, 1));
}
void SARInventoryItem::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	TextTest = FText::FromName("Leave");
	Icon = MyUIResources->GetBrush(FName("Inactive_Brush"));
	ImageColor = FSlateColor(FLinearColor(0, 1, 0, 1));
}
FReply SARInventoryItem::OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	TSharedPtr<FDragDropOperation> ops = MakeShareable(new FDragDropOperation);
	TSharedRef<FDragDropOperation> Op = ops.ToSharedRef();
	return FReply::Handled().BeginDragDrop(Op);
}

FReply SARInventoryItem::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	Icon = MyUIResources->GetBrush(FName("Active_Brush"));
	return FReply::Handled().DetectDrag(SharedThis(this), MouseEvent.GetEffectingButton()).CaptureMouse(SharedThis(this));
}

FReply SARInventoryItem::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled().ReleaseMouseCapture();
}

void SARInventoryItem::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
	
	if (ActionGrid.IsValid())
	{
		ActionGrid->ClearChildren();
	}

	for (int32 i = 0; i < 5; i++)
	{
		for (int32 j = 0; j < 10; j++)
		{
			ActionGrid->AddSlot(i, j)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(FText::FromName("Test"))
					.Font(FSlateFontInfo("Veranda", 16))
					//SNew(SARInventoryItem)
					//.OwnerHUD(OwnerHUD)
					//.MyPC(MyPC)
					//.Inventory(&ClonedInventory)
					//.OnMouseEnter(this, &SARButtonWidget::DoNothing)
					//.OnMouseLeave(this, &SARButtonWidget::DoNothing)
				];
		}
	}

	//if (TileView.IsValid())
	//{
	//	TileView->RequestListRefresh();
	//}
	//TWeakObjectPtr<AARPlayerController> MyPC = Cast<AARPlayerController>(OwnerHUD->GetOwningPlayerController());
	//if (MyPC.IsValid())
	//{
		//Inventory = &MyPC->Equipment->Inventory;
	//}
}
FReply SARInventoryItem::Drag(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled();
}
TSharedRef<ITableRow> SARInventoryItem::MakeTileViewWidget(TSharedPtr<FARItemInfo> AssetItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (AssetItem.IsValid())
	{
		TSharedRef< STableRow<TSharedPtr<FARItemInfo> >> ReturnRow = SNew(STableRow<TSharedPtr<FARItemInfo>>, OwnerTable)
			//.OnDragDetected(this, &SARInventoryItem::Drag)
			.Content()
			[
				SNew(STextBlock)
				.Text(FText::FromName(AssetItem->ItemName))
				//SNew(SARInventoryItem)
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

FText SARInventoryItem::GetTextFromInv() const
{
	//for (auto it; it = Inventory->CreateIterator(); ++it)
	//{

	//}
	//for (const FARItemInfo& item : Inventory)
	//{
	//	return FText::FromName(item.ItemName);
	//}
	return FText::FromName("No item");
}

const FSlateBrush* SARInventoryItem::GetImage() const
{
	return Icon;
}
FSlateColor SARInventoryItem::GetColor() const
{
	return ImageColor;
}




void FMyUIResources::Initialize()
{
	if (!MyUIResources.IsValid())
	{
		MyUIResources = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*MyUIResources);
	}
}

TSharedPtr<FSlateGameResources> FMyUIResources::GetSlateGameResources()
{
	return MyUIResources;
}

TSharedRef<class FSlateGameResources> FMyUIResources::Create()
{
	return FSlateGameResources::New(FName("MyUIResources"), m_Path, m_Path);
}

/*Unregister resources/styles with Slate, cleanup, free memory*/
void FMyUIResources::Shutdown()
{
	//Unregister *MyUIResources with Slate
	FSlateStyleRegistry::UnRegisterSlateStyle(*MyUIResources);

	//Debugging
	ensure(MyUIResources.IsUnique());

	//Removes reference to resources, decrements refcount, destroys resources if refcount=0
	//Do this to all SharedPtrs on Shutdown() or SomethingSimilar() to avoid memory leak
	MyUIResources.Reset();
}