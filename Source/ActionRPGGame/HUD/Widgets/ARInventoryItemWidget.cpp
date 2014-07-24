// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../../Types/ARStructTypes.h"
#include "../../ARPlayerController.h"
#include "../../ARCharacter.h"
#include "../../Items/ARItem.h"
#include "../../Items/ARWeapon.h"
#include "../../Componenets/AREquipmentComponent.h"
#include "../../Items/ARInventoryComponent.h"

#include "SlateDynamicImageBrush.h"

#include "ARInventoryItemWidget.h"
SARInventoryItemWidget::~SARInventoryItemWidget()
{
	if (ItemInSlot.IsValid())
	{
		ItemInSlot->Destroy();
		ItemInSlot.Reset();
	}
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
	//ItemInSlot = InArgs._ItemInSlot;
	EquipmentSlot = InArgs._EquipmentSlot;
	InventoryItem = InArgs._InventoryItem;

	if (PlayerController != NULL && InventoryItem.IsValid())
	{
		if (ItemInSlot.IsValid())
		{
			ItemInSlot->Destroy();
			ItemInSlot.Reset();
		}
		FString usless;
		FARItemData* data = nullptr;
		switch (InventoryItem->ItemSlot)
		{
		case EItemSlot::Item_Chest:
			data = ChestItemDataTable->FindRow<FARItemData>(InventoryItem->ItemID, usless);
			break;
		case EItemSlot::Item_Weapon:
			data = WeaponItemDataTable->FindRow<FARItemData>(InventoryItem->ItemID, usless);
			break;
		default:
			break;
		}
		if (data)
		{
			UBlueprint* gen = LoadObject<UBlueprint>(NULL, *data->ItemBlueprint.ToStringReference().ToString(), NULL, LOAD_None, NULL);
			if (gen)
			{

				FActorSpawnParameters SpawnInfo;
				SpawnInfo.bNoCollisionFail = true;
				SpawnInfo.Owner = PlayerController->GetPawn();
				ItemInSlot = PlayerController->GetWorld()->SpawnActor<AARItem>(gen->GeneratedClass, SpawnInfo);
				ItemInSlot->SetOwner(PlayerController->GetPawn());
				ItemInSlot->Instigator = PlayerController->GetPawn();
				ItemInSlot->ItemID = InventoryItem->ItemID;
			}
		}
		//if (data)
		//{
		//	delete data;
		//	data = nullptr;
		//}
	}

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
	if (ItemInSlot.IsValid())
	{
		return &ItemInSlot->ItemIcon;
	}
	//should return default image.
	return nullptr;
}
FText SARInventoryItemWidget::GetItemText() const
{
	if (InventoryItem.IsValid())
	{
		return FText::FromName(InventoryItem->ItemID);
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
	TSharedPtr<FInventoryDragDrop> Operation = DragDropEvent.GetOperationAs<FInventoryDragDrop>();
	if (this->EquipmentSlot == EEquipmentSlot::Item_Chest)
	{
		if (Equipment.IsValid() && Operation.IsValid())
		{
			int32 tempSlotID = Operation->PickedItem->SlotID;
			Operation->PickedItem->EEquipmentSlot = EEquipmentSlot::Item_Chest;
			Operation->LastItemSlot->ItemInSlot.Reset();
			Operation->LastItemSlot->InventoryItem.Reset();
			InventoryItem = Operation->PickedItem;
			ItemInSlot = Operation->InventoryItemObj;
			Equipment->ChangeItem(*Operation->PickedItem, tempSlotID);

			//Operation->LastItemSlot->PlayerController->RemoveItemFromInventory(Operation->LastItemSlot->InventoryItem->ItemID, Operation->LastItemSlot->InventoryItem->SlotID);
			return FReply::Handled();
		}
	}
	//check type of current InventorySlot type.
	if (this->EquipmentSlot == EEquipmentSlot::Item_LeftHandOne)
	{
		if (Equipment.IsValid() && Operation.IsValid())
		{
			//check last slot from which item has be dragged.
			//if it is Item_RightHandOne we will remove this item from
			// PlayerController->RightHandWeapons (TArray)
			//this probably should also check against inventory slot, to remove
			//item from inventory.
			//this is RPC call!
			if (Operation->PickedItem->EEquipmentSlot == EEquipmentSlot::Item_RightHandOne)
			{
				Equipment->RemoveRightHandWeapon(Operation->PickedItem->ItemID, Operation->PickedItem->SlotID);

					Equipment->UnEquipRightHandWeapon(Operation->PickedItem->ItemID);

			}
			//store current slot ID;
			int32 tempSlotID = this->InventoryItem->SlotID;
			//Change slot to which item is equiped.
			Operation->PickedItem->EEquipmentSlot = EEquipmentSlot::Item_LeftHandOne;

			//Assign new item to inventory.
			InventoryItem = Operation->PickedItem;
			//Assign actor to current slot.
			ItemInSlot = Operation->InventoryItemObj;

			//create new item
			FInventorySlot item;
			item.SlotID = Operation->PickedItem->SlotID;
			item.OldSlotID = Operation->LastItemSlot->InventoryItem->SlotID;
			item.ItemID = Operation->PickedItem->ItemID;
			item.ItemSlot = Operation->PickedItem->ItemSlot;
			item.EEquipmentSlot = Operation->PickedItem->EEquipmentSlot;

			Operation->LastItemSlot->ItemInSlot.Reset();
			Operation->LastItemSlot->InventoryItem.Reset();

			//add it to inventory.
			Equipment->AddLeftHandWeapon(item, tempSlotID);

			//Operation->LastItemSlot->PlayerController->RemoveItemFromInventory(Operation->LastItemSlot->InventoryItem->ItemID, Operation->LastItemSlot->InventoryItem->SlotID);
			return FReply::Handled();

		}
	}
	if (this->EquipmentSlot == EEquipmentSlot::Item_RightHandOne)
	{
		if (Equipment.IsValid() && Operation.IsValid())
		{
			if (Operation->PickedItem->EEquipmentSlot == EEquipmentSlot::Item_LeftHandOne)
			{
				Equipment->RemoveLeftHandWeapon(Operation->PickedItem->ItemID, Operation->PickedItem->SlotID);

				Equipment->UnEquipLeftHandWeapon(Operation->PickedItem->ItemID);

			}
			int32 tempSlotID = this->InventoryItem->SlotID;
			Operation->PickedItem->EEquipmentSlot = EEquipmentSlot::Item_RightHandOne;

			InventoryItem = Operation->PickedItem;
			ItemInSlot = Operation->InventoryItemObj;

			FInventorySlot item;
			item.SlotID = Operation->PickedItem->SlotID;
			item.OldSlotID = Operation->LastItemSlot->InventoryItem->SlotID;
			item.ItemID = Operation->PickedItem->ItemID;
			item.ItemSlot = Operation->PickedItem->ItemSlot;
			item.EEquipmentSlot = Operation->PickedItem->EEquipmentSlot;

			Operation->LastItemSlot->ItemInSlot.Reset();
			Operation->LastItemSlot->InventoryItem.Reset();

			Equipment->AddRightHandWeapon(item, tempSlotID);


			//Operation->LastItemSlot->PlayerController->RemoveItemFromInventory(Operation->LastItemSlot->InventoryItem->ItemID, Operation->LastItemSlot->InventoryItem->SlotID);
			return FReply::Handled();

		}
	}
	if (this->EquipmentSlot == EEquipmentSlot::Item_Inventory)
	{
		if (Inventory.IsValid() && Operation.IsValid())
		{
			int32 tempSlot = InventoryItem->SlotID;
			InventoryItem = Operation->PickedItem;
			ItemInSlot = Operation->InventoryItemObj;
			FInventorySlot item;
			item.SlotID = Operation->PickedItem->SlotID;
			item.OldSlotID = Operation->LastItemSlot->InventoryItem->SlotID;
			item.ItemID = Operation->PickedItem->ItemID;
			item.ItemSlot = Operation->PickedItem->ItemSlot;
			item.EEquipmentSlot = Operation->PickedItem->EEquipmentSlot;

			Inventory->AddItemToInventoryOnSlot(item, tempSlot);

			if (Operation->LastItemSlot->SlotType != EItemSlot::Item_Inventory)
			{

				if (Equipment.IsValid())
				{
					Equipment->UnEquipItem(*InventoryItem);
				}
				Operation->LastItemSlot->InventoryItem.Reset();
				//InventoryItem = NULL
				Operation->LastItemSlot->ItemInSlot.Reset();
			}
			Operation->LastItemSlot->ItemInSlot.Reset();
			Operation->LastItemSlot->InventoryItem.Reset();
			return FReply::Handled();
		}
	}
	Operation.Reset();
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
	if (InventoryItem->ItemID.IsNone())
	{
		return FReply::Unhandled();
	}
	TSharedRef<FInventoryDragDrop> Operation = FInventoryDragDrop::New(InventoryItem, ItemInSlot, SharedThis(this));
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
		if (!InventoryItem->ItemID.IsNone())
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



TSharedRef<FInventoryDragDrop> FInventoryDragDrop::New(TSharedPtr<FInventorySlot> PickedItemIn, TWeakObjectPtr<class AARItem> InventoryItemIn, TSharedPtr<SARInventoryItemWidget> LastItemSlotIn)
{
	TSharedRef<FInventoryDragDrop> Operation = MakeShareable(new FInventoryDragDrop);

	Operation->PickedItem = PickedItemIn;
	Operation->InventoryItemObj = InventoryItemIn;
	Operation->LastItemSlot = LastItemSlotIn;
	Operation->Construct();
	return Operation;
}

TSharedPtr<SWidget> FInventoryDragDrop::GetDefaultDecorator() const
{
	return SNew(SBorder).Cursor(EMouseCursor::GrabHandClosed)
		.Content()
		[
			//SNew(STextBlock)
			//.ShadowColorAndOpacity(FLinearColor::Black)
			//	.ColorAndOpacity(FLinearColor::White)
			//	.ShadowOffset(FIntPoint(-1, 1))
			//	.Font(FSlateFontInfo("Veranda", 16)) //don't believe this works, see Rama's tutorial
			//	//.Text(FText::FromName(PickedItem->ItemName))
			//	.Text(FText::FromName("Picked.Item"))
			SNew(SImage)
			.Image(this, &FInventoryDragDrop::GetDecoratorIcon)
		];
}

const FSlateBrush* FInventoryDragDrop::GetDecoratorIcon() const
{
	FSlateBrush* image = NULL;

	if (InventoryItemObj.IsValid())
	{
		image = &InventoryItemObj->ItemIcon;
	}

	return image;
}