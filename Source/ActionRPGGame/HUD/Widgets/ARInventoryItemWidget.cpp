// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../../Types/ARStructTypes.h"
#include "../../ARPlayerController.h"
#include "../../ARCharacter.h"
#include "../../Items/ARItem.h"
#include "../../Items/ARWeapon.h"
#include "../../Componenets/AREquipmentComponent.h"
#include "../../Items/ARInventoryComponent.h"
#include "../../Items/ARItemPickup.h"

#include "SlateDynamicImageBrush.h"

#include "ARInventoryItemWidget.h"
SARInventoryItemWidget::~SARInventoryItemWidget()
{

}
void SARInventoryItemWidget::Construct(const FArguments& InArgs)
{
	PlayerController = InArgs._PlayerController;
	InventoryItem = InArgs._InventoryItem;
	Inventory = InArgs._Inventory;
	Equipment = InArgs._Equipment;

	TextColor = FSlateColor(FLinearColor(1, 0, 0, 1));
	SlotType = InArgs._SlotType;
	SlotName = InArgs._SlotName;
	//	EquipmentSlot = InArgs._EquipmentSlot;
	InventoryItem = InArgs._InventoryItem;
	LootedObject = InArgs._LootedObject;

	ItemInThisSlot = nullptr;

	SpawnItem();

	ChildSlot
		[

			SNew(SBorder)
			.BorderBackgroundColor(FSlateColor(FLinearColor(1, 0, 0, 1)))
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				[
					SNew(SImage)
					.Image(this, &SARInventoryItemWidget::GetImage)
				]
				+ SOverlay::Slot()
					[
						SNew(STextBlock)
						.Text(SlotName)
					]
			]
		];
}

const FSlateBrush* SARInventoryItemWidget::GetImage() const
{
	if (ItemInThisSlot)
	{
		return &ItemInThisSlot->ItemIcon;
	}
	//should return default image.
	return nullptr;
}
FText SARInventoryItemWidget::GetItemText() const
{
	if (InventoryItem.IsValid())
	{
		//	return FText::FromName(InventoryItem->ItemID);
	}

	return SlotName;
}
FSlateColor SARInventoryItemWidget::GetTextColor() const
{
	return TextColor;
}
void SARInventoryItemWidget::OnDragEnter(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	TSharedPtr<FDragDropOperation> Operation = DragDropEvent.GetOperationAs<FInventoryDragDrop>();
}
void SARInventoryItemWidget::OnDragLeave(const FDragDropEvent& DragDropEvent)
{
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
	/*
		this entire thing is pretty unsafe, and there possibility of duping items.
		There will be needed strong server side validation, to make sure, that player
		operate on items, which are currently possed in inventory.
		*/
	TSharedPtr<FInventoryDragDrop> Operation = DragDropEvent.GetOperationAs<FInventoryDragDrop>();
	if (Operation->PickedItem->ItemSlot == EItemSlot::Item_Weapon
		|| Operation->PickedItem->ItemSlot == EItemSlot::Item_TwoHanded)
	{

	}
	if (InventoryItem->DragDropSlot == EDragDropSlot::LeftHand)
	{
		if (Equipment.IsValid() && Operation.IsValid() && Inventory.IsValid())
		{
			if (Operation->PickedItem->DragDropSlot == EDragDropSlot::RightHand)
			{
				Equipment->RemoveWeapon(Operation->PickedItem->ItemKey, Operation->PickedItem->SlotIndex, 1);
				Equipment->UnEquipWeapon(Operation->PickedItem->ItemKey, 1);
			}
			else if (Operation->PickedItem->DragDropSlot == EDragDropSlot::LeftHand)
			{
				Equipment->RemoveWeapon(Operation->PickedItem->ItemKey, Operation->PickedItem->SlotIndex, 0);
				Equipment->UnEquipWeapon(Operation->PickedItem->ItemKey, 0);
			}
			else if (Operation->PickedItem->DragDropSlot == EDragDropSlot::Inventory)
			{
				Inventory->RemoveItemFromInventory(Operation->PickedItem->ItemKey, Operation->PickedItem->SlotIndex);
			}

			FARDragDropInfo dropInfo;
			dropInfo.OldSlotIndex = Operation->PickedItem->SlotIndex;
			dropInfo.ItemKey = Operation->PickedItem->ItemKey;
			dropInfo.ItemIndex = Operation->PickedItem->ItemIndex;
			dropInfo.ItemSlot = Operation->PickedItem->ItemSlot;
			dropInfo.DragDropSlot = EDragDropSlot::LeftHand;

			Equipment->AddWeapon(dropInfo, InventoryItem->SlotIndex, 0);

			return FReply::Handled();
		}
	}
	if (InventoryItem->DragDropSlot == EDragDropSlot::RightHand)
	{
		if (Equipment.IsValid() && Operation.IsValid() && Inventory.IsValid())
		{
			if (Operation->PickedItem->DragDropSlot == EDragDropSlot::LeftHand)
			{
				Equipment->RemoveWeapon(Operation->PickedItem->ItemKey, Operation->PickedItem->SlotIndex, 0);
				Equipment->UnEquipWeapon(Operation->PickedItem->ItemKey, 1);
			}
			else if (Operation->PickedItem->DragDropSlot == EDragDropSlot::RightHand)
			{
				Equipment->RemoveWeapon(Operation->PickedItem->ItemKey, Operation->PickedItem->SlotIndex, 1);
				Equipment->UnEquipWeapon(Operation->PickedItem->ItemKey, 0);
			}
			else if (Operation->PickedItem->DragDropSlot == EDragDropSlot::Inventory)
			{
				Inventory->RemoveItemFromInventory(Operation->PickedItem->ItemKey, Operation->PickedItem->SlotIndex);
			}

			FARDragDropInfo dropInfo;
			dropInfo.OldSlotIndex = Operation->PickedItem->SlotIndex;
			dropInfo.ItemKey = Operation->PickedItem->ItemKey;
			dropInfo.ItemIndex = Operation->PickedItem->ItemIndex;
			dropInfo.ItemSlot = Operation->PickedItem->ItemSlot;
			dropInfo.DragDropSlot = EDragDropSlot::RightHand;

			Equipment->AddWeapon(dropInfo, InventoryItem->SlotIndex, 1);

			return FReply::Handled();
		}
	}

	if (InventoryItem->DragDropSlot == EDragDropSlot::Inventory)
	{
		if (Inventory.IsValid())
		{
			FARDragDropInfo dropInfo;
			dropInfo.ItemKey = Operation->PickedItem->ItemKey;
			dropInfo.ItemIndex = Operation->PickedItem->ItemIndex;
			dropInfo.ItemSlot = Operation->PickedItem->ItemSlot;

			if (Operation->PickedItem->DragDropSlot == EDragDropSlot::Inventory)
			{
				dropInfo.OldSlotIndex = Operation->PickedItem->SlotIndex;
				dropInfo.DragDropSlot = EDragDropSlot::Inventory;
				Inventory->AddItemToInventoryOnSlot(dropInfo, InventoryItem->SlotIndex);
				return FReply::Handled();
			}
			else if (Operation->PickedItem->DragDropSlot != EDragDropSlot::Inventory)
			{
				if (Equipment.IsValid())
				{
					if (Operation->PickedItem->DragDropSlot == EDragDropSlot::LeftHand)
					{
						Equipment->RemoveWeapon(Operation->PickedItem->ItemKey, Operation->PickedItem->SlotIndex, 0);
						Equipment->UnEquipItem(*InventoryItem);
					}
					if (Operation->PickedItem->DragDropSlot == EDragDropSlot::RightHand)
					{
						Equipment->RemoveWeapon(Operation->PickedItem->ItemKey, Operation->PickedItem->SlotIndex, 1);
						Equipment->UnEquipItem(*InventoryItem);
					}
				}
				dropInfo.OldSlotIndex = Operation->PickedItem->SlotIndex;
				//dropInfo.DragDropSlot = EDragDropSlot::Inventory;
				Inventory->AddItemToInventoryOnSlot(dropInfo, InventoryItem->SlotIndex);
				return FReply::Handled();
			}
		}
	}

	Operation.Reset();
	return FReply::Unhandled();
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
	if (InventoryItem->ItemKey.IsNone())
	{
		return FReply::Unhandled();
	}
	TSharedRef<FInventoryDragDrop> Operation = FInventoryDragDrop::New(this->InventoryItem, SharedThis(this));
	Operation->SetDecoratorVisibility(true);
	//InventoryItem.Reset();
	//InventoryItem = NULL;
	//ItemInSlot->Destroy();
	//ItemInSlot.Reset();

	return FReply::Handled().BeginDragDrop(Operation);
}

FReply SARInventoryItemWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (InventoryItem.IsValid())
	{
		if (!InventoryItem->ItemKey.IsNone())
		{
			return FReply::Handled().DetectDrag(SharedThis(this), MouseEvent.GetEffectingButton()).CaptureMouse(SharedThis(this));
		}
	}
	return FReply::Unhandled();
}
FReply SARInventoryItemWidget::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled().ReleaseMouseCapture();
}


void SARInventoryItemWidget::SpawnItem()
{
	if (PlayerController != NULL && InventoryItem.IsValid() && TestItems && TestItems->EditEntries.Num() > 0
		&& InventoryItem->ItemIndex >= 0)
	{
		if (InventoryItem->ItemSlot == EItemSlot::Item_Chest)
		{
			ItemInThisSlot = ChestItems->GetItemDataFromArrayPtr(InventoryItem->ItemIndex);
			return;
		}
		ItemInThisSlot = TestItems->GetItemDataFromArrayPtr(InventoryItem->ItemIndex);
	}
}


TSharedRef<FInventoryDragDrop> FInventoryDragDrop::New(TSharedPtr<FARDragDropInfo> PickedItemIn, TSharedPtr<SARInventoryItemWidget> LastItemSlotIn)
{
	TSharedRef<FInventoryDragDrop> Operation = MakeShareable(new FInventoryDragDrop);
	Operation->ItemInThisSlot = nullptr;
	Operation->PickedItem = PickedItemIn;
	Operation->LastItemSlot = LastItemSlotIn;
	if (TestItems && TestItems->EditEntries.Num() > 0 && PickedItemIn->ItemIndex >= 0)
	{
		if (PickedItemIn->ItemSlot == EItemSlot::Item_Chest)
		{
			Operation->ItemInThisSlot = ChestItems->GetItemDataFromArrayPtr(PickedItemIn->ItemIndex);
		}
		else
			Operation->ItemInThisSlot = TestItems->GetItemDataFromArrayPtr(PickedItemIn->ItemIndex);
	}

	Operation->Construct();
	return Operation;
}

TSharedPtr<SWidget> FInventoryDragDrop::GetDefaultDecorator() const
{
	return SNew(SBorder).Cursor(EMouseCursor::GrabHandClosed)
		.Content()
		[
			SNew(SImage)
			.Image(this, &FInventoryDragDrop::GetDecoratorIcon)
		];
}

const FSlateBrush* FInventoryDragDrop::GetDecoratorIcon() const
{
	FSlateBrush* image = NULL;

	if (ItemInThisSlot)
	{
		image = &ItemInThisSlot->ItemIcon;
	}

	return image;
}