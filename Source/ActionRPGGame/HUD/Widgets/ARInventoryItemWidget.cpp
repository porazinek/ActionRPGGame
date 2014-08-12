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
	if (InventoryItem->DragDropSlot == EDragDropSlot::LeftHand)
	{
		if (Equipment.IsValid() && Operation.IsValid() && Inventory.IsValid())
		{
			if (Operation->PickedItem->DragDropSlot == EDragDropSlot::RightHand)
			{
				Equipment->RemoveWeapon(Operation->PickedItem->ItemKey, Operation->PickedItem->SlotIndex, 1);
				Equipment->UnEquipWeapon(Operation->PickedItem->ItemKey, 1);
			}
			Inventory->RemoveItemFromInventory(Operation->PickedItem->ItemKey, Operation->PickedItem->SlotIndex);

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
					Equipment->RemoveWeapon(Operation->PickedItem->ItemKey, Operation->PickedItem->SlotIndex, 1);
					Equipment->UnEquipItem(*InventoryItem);
				}
				dropInfo.OldSlotIndex = Operation->PickedItem->SlotIndex;
				//dropInfo.DragDropSlot = EDragDropSlot::Inventory;
				Inventory->AddItemToInventoryOnSlot(dropInfo, InventoryItem->SlotIndex);
				return FReply::Handled();
			}
		}
	}

	//if (this->EquipmentSlot == EEquipmentSlot::Item_Chest)
	//{
	//	if (Equipment.IsValid() && Operation.IsValid())
	//	{
	//		int32 tempSlotID = Operation->PickedItem->SlotID;
	//		Operation->PickedItem->EEquipmentSlot = EEquipmentSlot::Item_Chest;
	//		Operation->LastItemSlot->InventoryItem.Reset();
	//		InventoryItem = Operation->PickedItem;
	//		Equipment->ChangeItem(*Operation->PickedItem, tempSlotID);

	//		//Operation->LastItemSlot->PlayerController->RemoveItemFromInventory(Operation->LastItemSlot->InventoryItem->ItemID, Operation->LastItemSlot->InventoryItem->SlotID);
	//		return FReply::Handled();
	//	}
	//}
	//if (this->EquipmentSlot == EEquipmentSlot::Item_LootWindow)
	//{
	//	return FReply::Unhandled();
	//}


	////check type of current InventorySlot type.
	//if (this->EquipmentSlot == EEquipmentSlot::Item_LeftHandOne)
	//{
	//	if (Equipment.IsValid() && Operation.IsValid())
	//	{
	//		//check last slot from which item has been dragged.
	//		//if it is Item_RightHandOne we will remove this item from
	//		// Equipment->RightHandWeapons (TArray)
	//		//this probably should also check against inventory slot, to remove
	//		//item from inventory.
	//		//this is RPC call!
	//		if (Operation->PickedItem->EEquipmentSlot == EEquipmentSlot::Item_RightHandOne)
	//		{
	//			Equipment->RemoveWeapon(Operation->PickedItem->ItemID, Operation->PickedItem->SlotID, 1);
	//			Equipment->UnEquipWeapon(Operation->PickedItem->ItemID, 1);
	//		}
	//		Inventory->RemoveItemFromInventory(Operation->PickedItem->ItemID, Operation->PickedItem->SlotID);
	//		//store current slot ID;
	//		int32 tempSlotID = this->InventoryItem->SlotID;
	//		//Change slot to which item is equiped.
	//		Operation->PickedItem->EEquipmentSlot = EEquipmentSlot::Item_LeftHandOne;

	//		//Assign new item to inventory.
	//		InventoryItem = Operation->PickedItem;

	//		//create new item
	//		FInventorySlot item;
	//		item.SlotID = Operation->PickedItem->SlotID;
	//		item.OldSlotID = Operation->LastItemSlot->InventoryItem->SlotID;
	//		item.ItemID = Operation->PickedItem->ItemID;
	//		item.ItemIndex = Operation->PickedItem->ItemIndex;
	//		item.ItemSlot = Operation->PickedItem->ItemSlot;
	//		//item.EEquipmentSlot = Operation->PickedItem->EEquipmentSlot;
	//		item.EEquipmentSlot = EEquipmentSlot::Item_LeftHandOne;
	//		Operation->LastItemSlot->InventoryItem.Reset();

	//		//add it to inventory.
	//		Equipment->AddWeapon(item, tempSlotID, 0);

	//		//Operation->LastItemSlot->PlayerController->RemoveItemFromInventory(Operation->LastItemSlot->InventoryItem->ItemID, Operation->LastItemSlot->InventoryItem->SlotID);
	//		return FReply::Handled();

	//	}
	//}
	//if (this->EquipmentSlot == EEquipmentSlot::Item_RightHandOne)
	//{
	//	if (Equipment.IsValid() && Operation.IsValid())
	//	{
	//		if (Operation->PickedItem->EEquipmentSlot == EEquipmentSlot::Item_LeftHandOne)
	//		{
	//			Equipment->RemoveWeapon(Operation->PickedItem->ItemID, Operation->PickedItem->SlotID, 0);
	//			
	//			Equipment->UnEquipWeapon(Operation->PickedItem->ItemID, 0);
	//		}
	//		Inventory->RemoveItemFromInventory(Operation->PickedItem->ItemID, Operation->PickedItem->SlotID);
	//		int32 tempSlotID = Operation->LastItemSlot->InventoryItem->SlotID;
	//		if (InventoryItem.IsValid())
	//			tempSlotID = this->InventoryItem->SlotID;
	//		Operation->PickedItem->EEquipmentSlot = EEquipmentSlot::Item_RightHandOne;

	//		InventoryItem = Operation->PickedItem;

	//		FInventorySlot item;
	//		item.SlotID = Operation->PickedItem->SlotID;
	//		item.OldSlotID = Operation->LastItemSlot->InventoryItem->SlotID;
	//		item.ItemID = Operation->PickedItem->ItemID;
	//		item.ItemIndex = Operation->PickedItem->ItemIndex;
	//		item.ItemSlot = Operation->PickedItem->ItemSlot;
	//		//item.EEquipmentSlot = Operation->PickedItem->EEquipmentSlot;
	//		item.EEquipmentSlot = EEquipmentSlot::Item_RightHandOne;
	//		Operation->LastItemSlot->InventoryItem.Reset();

	//		Equipment->AddWeapon(item, tempSlotID, 1);


	//		//Operation->LastItemSlot->PlayerController->RemoveItemFromInventory(Operation->LastItemSlot->InventoryItem->ItemID, Operation->LastItemSlot->InventoryItem->SlotID);
	//		return FReply::Handled();

	//	}
	//}
	//if (this->EquipmentSlot == EEquipmentSlot::Item_Inventory)
	//{
	//	if (Inventory.IsValid() && Operation.IsValid() && Equipment.IsValid())
	//	{
	//		int32 oterSlot = this->InventoryItem->SlotID;
	//		int32 tempSlot = Operation->LastItemSlot->InventoryItem->SlotID;
	//		if (InventoryItem.IsValid())
	//			tempSlot = InventoryItem->SlotID;

	//		if (Operation->LastItemSlot->InventoryItem->EEquipmentSlot == EEquipmentSlot::Item_RightHandOne)
	//		{
	//			Equipment->RemoveWeapon(Operation->PickedItem->ItemID, Operation->PickedItem->SlotID, 1);
	//			Equipment->UnEquipWeapon(Operation->PickedItem->ItemID, 1);
	//		}
	//		else if (Operation->LastItemSlot->InventoryItem->EEquipmentSlot == EEquipmentSlot::Item_LeftHandOne)
	//		{
	//			Equipment->RemoveWeapon(Operation->PickedItem->ItemID, Operation->PickedItem->SlotID, 0);
	//			Equipment->UnEquipWeapon(Operation->PickedItem->ItemID, 0);
	//		}

	//		InventoryItem = Operation->PickedItem;
	//		
	//		//ItemInThisSlot = Operation->ItemInThisSlot;
	//		FInventorySlot item;
	//		item.SlotID = Operation->PickedItem->SlotID;
	//		item.OldSlotID = Operation->PickedItem->SlotID;

	//		item.ItemID = Operation->PickedItem->ItemID;
	//		/*
	//			I probably shouldn't give client id's of items.
	//			But somehow figure them out on server side.
	//		*/
	//		item.ItemIndex = Operation->PickedItem->ItemIndex;
	//		item.ItemSlot = Operation->PickedItem->ItemSlot;

	//		item.EEquipmentSlot = Operation->LastItemSlot->EquipmentSlot;
	//		if (Operation->PickedItem->EEquipmentSlot == EEquipmentSlot::Item_LootWindow)
	//		{
	//			if (Operation->LastItemSlot->LootedObject.IsValid())
	//			{
	//				item.EEquipmentSlot = EEquipmentSlot::Item_Inventory;
	//				Operation->LastItemSlot->LootedObject->GiveItemToPlayer(item, PlayerController.Get(), oterSlot);
	//			}
	//		}
	//		else
	//		{
	//			Inventory->AddItemToInventoryOnSlot(item, tempSlot);
	//		}

	//		if (Operation->LastItemSlot->SlotType != EItemSlot::Item_Inventory)
	//		{
	//			if (Equipment.IsValid())
	//			{
	//				Equipment->UnEquipItem(*InventoryItem);
	//			}
	//			Operation->LastItemSlot->InventoryItem.Reset();
	//		}
	//		Operation->LastItemSlot->InventoryItem.Reset();
	//		return FReply::Handled();
	//	}
	//}
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