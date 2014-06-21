// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../../Types/ARStructTypes.h"
#include "../../ARPlayerController.h"
#include "../../ARCharacter.h"
#include "../../Items/ARItem.h"
#include "../../Items/ARWeapon.h"
#include "../../Componenets/AREquipmentComponent.h"

#include "SlateDynamicImageBrush.h"

#include "ARInventoryItemWidget.h"

void SARInventoryItemWidget::Construct(const FArguments& InArgs)
{
	PlayerController = InArgs._PlayerController;
	InventoryItem = InArgs._InventoryItem;
	TextColor = FSlateColor(FLinearColor(1, 0, 0, 1));
	SlotType = InArgs._SlotType;
	SlotName = InArgs._SlotName;
	ItemInSlot = InArgs._ItemInSlot;
	EquipmentSlot = InArgs._EquipmentSlot;
	InventoryItemObj = InArgs._InventoryItemObj;

	ChildSlot
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				SNew(STextBlock)
				.Text(this, &SARInventoryItemWidget::GetItemText)
				.ColorAndOpacity(this, &SARInventoryItemWidget::GetTextColor)
				//SNew(SImage)
				//.Image(this, &SARInventoryItemWidget::GetImage)
			]
		];
}

const FSlateBrush* SARInventoryItemWidget::GetImage() const
{
	FSlateBrush* image = NULL;

	if (InventoryItem.IsValid())
	{
		image = &InventoryItem->SlateIcon;
	}

	return image;
}
FText SARInventoryItemWidget::GetItemText() const
{
	if (InventoryItem.IsValid())
	{
		return FText::FromName(InventoryItem->ItemName);
	}

	return SlotName;
}
FSlateColor SARInventoryItemWidget::GetTextColor() const
{
	return TextColor;
}
void SARInventoryItemWidget::OnDragEnter(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	//TSharedPtr<FDragDropOperation> Operation = DragDropEvent.GetOperation();

	TSharedPtr<FDragDropOperation> Operation = DragDropEvent.GetOperationAs<FInventoryDragDrop>();
}
void SARInventoryItemWidget::OnDragLeave(const FDragDropEvent& DragDropEvent)
{
	//TSharedPtr<FInventoryDragDrop> Operation2 = DragDropEvent.GetOperationAs<FInventoryDragDrop>();

	//Operation2->LastItemSlot->InventoryItem = Operation2->PickedItem;
}
FReply SARInventoryItemWidget::OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	TSharedPtr<FInventoryDragDrop> Operation = DragDropEvent.GetOperationAs<FInventoryDragDrop>();
	if (Operation.IsValid())
	{
		Operation->SetDecoratorVisibility(true);
	}

	return FReply::Handled();
}
FReply SARInventoryItemWidget::OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{

	//TSharedPtr<SARInventoryItemWidget> Widget = MakeShareable(this);
	//we want to equip items only when dragged on proper slot (not inventory one!)
	if (this->SlotType != EItemSlot::Item_Inventory && this->SlotType != EItemSlot::Item_Weapon)
	{
		TSharedPtr<FInventoryDragDrop> Operation = DragDropEvent.GetOperationAs<FInventoryDragDrop>();
		if (PlayerController.IsValid() && Operation.IsValid())
		{
			TWeakObjectPtr<AARCharacter> MyChar = Cast<AARCharacter>(PlayerController->GetPawn());

			if (MyChar.IsValid())
			{
				//assign pointer to this slot on drop.
				InventoryItem = Operation->PickedItem;

				//for user feedback it would be best to create separate slot types for each item
				//though all will call the same function, and server will do checking 
				//if item fits to slot.
				MyChar->Equipment->ChangeItem(Operation->PickedItem->ItemName);
				return FReply::Handled();
			}
		}
	}

	if (this->SlotType == EItemSlot::Item_Weapon)
	{
		TSharedPtr<FInventoryDragDrop> Operation = DragDropEvent.GetOperationAs<FInventoryDragDrop>();
		if (PlayerController.IsValid() && Operation.IsValid())
		{
			TWeakObjectPtr<AARCharacter> MyChar = Cast<AARCharacter>(PlayerController->GetPawn());

			if (MyChar.IsValid())
			{
				//assign pointer to this slot on drop.
				InventoryItem = Operation->PickedItem;
				MyChar->Equipment->AddLeftHandWeapon(Operation->PickedItem->ItemName);
				return FReply::Handled();
			}
		}
	}

	if (this->SlotType == EItemSlot::Item_Inventory)
	{
		TSharedPtr<FInventoryDragDrop> Operation = DragDropEvent.GetOperationAs<FInventoryDragDrop>();
		if (PlayerController.IsValid() && Operation.IsValid())
		{
			int32 ItemIndex = 0;
			for (FARItemInfo& item : PlayerController->Inventory)
			{
				if (item.ItemID == InventoryItem->ItemID)
				{
					PlayerController->SwapItemPosition(*Operation->PickedItem, ItemIndex);
					ItemIndex = 0;
					return FReply::Handled();
					
				}
				ItemIndex++;
			}
		}
	}
	TSharedPtr<FInventoryDragDrop> Operation2 = DragDropEvent.GetOperationAs<FInventoryDragDrop>();
	
	Operation2->LastItemSlot->InventoryItem = Operation2->PickedItem;

	//if (this->SlotType == EItemSlot::Item_Chest)
	//{
	//	float flo = 10;
	//}
	return FReply::Handled();
}
void SARInventoryItemWidget::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	TextColor = FSlateColor(FLinearColor(0, 1, 0, 1));
}
void SARInventoryItemWidget::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	TextColor = FSlateColor(FLinearColor(1, 0, 0, 1));
}
FReply SARInventoryItemWidget::OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	//TSharedPtr<FDragDropOperation> Operation = MakeShareable(new FDragDropOperation);
	TSharedRef<FInventoryDragDrop> Operation = FInventoryDragDrop::New(InventoryItem, InventoryItemObj, SharedThis(this));
	Operation->SetDecoratorVisibility(true);
	InventoryItem.Reset();
	InventoryItem = NULL;
	return FReply::Handled().BeginDragDrop(Operation);
}

FReply SARInventoryItemWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled().DetectDrag(SharedThis(this), MouseEvent.GetEffectingButton()).CaptureMouse(SharedThis(this));
}
FReply SARInventoryItemWidget::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled().ReleaseMouseCapture();
}



TSharedRef<FInventoryDragDrop> FInventoryDragDrop::New(TSharedPtr<FARItemInfo> PickedItemIn, TWeakObjectPtr<UObject> InventoryItemObj, TSharedPtr<SARInventoryItemWidget> LastItemSlotIn)
{
	TSharedRef<FInventoryDragDrop> Operation = MakeShareable(new FInventoryDragDrop);

	Operation->PickedItem = PickedItemIn;
	Operation->InventoryItemObj = InventoryItemObj;
	Operation->LastItemSlot = LastItemSlotIn;
	Operation->Construct();
	return Operation;
}

TSharedPtr<SWidget> FInventoryDragDrop::GetDefaultDecorator() const
{
	return SNew(SBorder).Cursor(EMouseCursor::GrabHandClosed)
		.Content()
		[
			SNew(STextBlock)
			.ShadowColorAndOpacity(FLinearColor::Black)
				.ColorAndOpacity(FLinearColor::White)
				.ShadowOffset(FIntPoint(-1, 1))
				.Font(FSlateFontInfo("Veranda", 16)) //don't believe this works, see Rama's tutorial
				.Text(FText::FromName(PickedItem->ItemName))
			//SNew(SImage)
			//.Image(this, &FInventoryDragDrop::GetDecoratorIcon)
		];
}

const FSlateBrush* FInventoryDragDrop::GetDecoratorIcon() const
{
	FSlateBrush* image = NULL;

	if (PickedItem.IsValid())
	{
		image = &PickedItem->SlateIcon;
	}

	return image;
}