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
SARInventoryItemWidget::~SARInventoryItemWidget()
{
	if (ItemInSlot.IsValid())
	{
		ItemInSlot->Destroy();
	}
}
void SARInventoryItemWidget::Construct(const FArguments& InArgs)
{
	PlayerController = InArgs._PlayerController;
	InventoryItem = InArgs._InventoryItem;
	TextColor = FSlateColor(FLinearColor(1, 0, 0, 1));
	SlotType = InArgs._SlotType;
	SlotName = InArgs._SlotName;
	//ItemInSlot = InArgs._ItemInSlot;
	EquipmentSlot = InArgs._EquipmentSlot;
	InventoryItemObj = InArgs._InventoryItemObj;

	if (PlayerController != NULL && InventoryItemObj.IsValid())
	{
		if (ItemInSlot.IsValid())
		{
			ItemInSlot->Destroy();
			ItemInSlot.Reset();
		}
		FString usless;
		FARItemData* data = ChestItemDataTable->FindRow<FARItemData>(InventoryItemObj->ItemID, usless);
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
				ItemInSlot->ItemID = InventoryItemObj->ItemID;
			}
		}
	}

	ChildSlot
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				//SNew(STextBlock)
				//.Text(this, &SARInventoryItemWidget::GetItemText)
				//.ColorAndOpacity(this, &SARInventoryItemWidget::GetTextColor)
				SNew(SImage)
				.Image(this, &SARInventoryItemWidget::GetImage)
			]
		];
}

const FSlateBrush* SARInventoryItemWidget::GetImage() const
{
	FSlateBrush* image = NULL;

	if (ItemInSlot.IsValid())
	{
		image = &ItemInSlot->ItemIcon;
	}
	//should return default image.
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
	if (this->SlotType == EItemSlot::Item_Chest)
	{
		TSharedPtr<FInventoryDragDrop> Operation = DragDropEvent.GetOperationAs<FInventoryDragDrop>();
		if (PlayerController.IsValid() && Operation.IsValid())
		{
			TWeakObjectPtr<AARCharacter> MyChar = Cast<AARCharacter>(PlayerController->GetPawn());

			if (MyChar.IsValid())
			{
				int32 tempSlotID = Operation->PickedItem->SlotID;
				Operation->LastItemSlot->ItemInSlot.Reset();
				Operation->LastItemSlot->InventoryItemObj.Reset();
				InventoryItemObj = Operation->PickedItem;
				ItemInSlot = Operation->InventoryItemObj;
				MyChar->Equipment->ChangeItem(*Operation->PickedItem, tempSlotID);

				//Operation->LastItemSlot->PlayerController->RemoveItemFromInventory(Operation->LastItemSlot->InventoryItemObj->ItemID, Operation->LastItemSlot->InventoryItemObj->SlotID);
				return FReply::Handled();
			}
		}
	}
	if (this->SlotType == EItemSlot::Item_Inventory)
	{
		TSharedPtr<FInventoryDragDrop> Operation = DragDropEvent.GetOperationAs<FInventoryDragDrop>();
		if (PlayerController.IsValid() && Operation.IsValid())
		{
			int32 tempSlot = InventoryItemObj->SlotID;
			InventoryItemObj = Operation->PickedItem;
			ItemInSlot = Operation->InventoryItemObj;
			FInventorySlot item;
			item.SlotID = Operation->PickedItem->SlotID;
			item.ItemID = Operation->PickedItem->ItemID;
			item.ItemSlot = Operation->PickedItem->ItemSlot;
			item.EEquipmentSlot = Operation->PickedItem->EEquipmentSlot;

			PlayerController->AddItemToInventoryOnSlot(item, tempSlot);

			if (Operation->LastItemSlot->SlotType != EItemSlot::Item_Inventory)
			{
				const TWeakObjectPtr<AARCharacter> MyChar = Cast<AARCharacter>(PlayerController->GetPawn());
				if (MyChar.IsValid())
				{
					MyChar->Equipment->UnEquipItem(*InventoryItemObj);
				}
				Operation->LastItemSlot->InventoryItemObj.Reset();
				//InventoryItemObj = NULL
				Operation->LastItemSlot->ItemInSlot.Reset();
			}
			return FReply::Handled();
		}
	}
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
	TSharedRef<FInventoryDragDrop> Operation = FInventoryDragDrop::New(InventoryItemObj, ItemInSlot, SharedThis(this));
	Operation->SetDecoratorVisibility(true);
	//InventoryItemObj.Reset();
	//InventoryItemObj = NULL;
	//ItemInSlot->Destroy();
	//ItemInSlot.Reset();

	return FReply::Handled().BeginDragDrop(Operation);
}

FReply SARInventoryItemWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (InventoryItemObj->ItemID != "-1")
	{
		return FReply::Handled().DetectDrag(SharedThis(this), MouseEvent.GetEffectingButton()).CaptureMouse(SharedThis(this));
	}
	return FReply::Unhandled();
}
FReply SARInventoryItemWidget::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled().ReleaseMouseCapture();
}



TSharedRef<FInventoryDragDrop> FInventoryDragDrop::New(TSharedPtr<FInventorySlot> PickedItemIn, TWeakObjectPtr<class AARItem> InventoryItemObjIn, TSharedPtr<SARInventoryItemWidget> LastItemSlotIn)
{
	TSharedRef<FInventoryDragDrop> Operation = MakeShareable(new FInventoryDragDrop);

	Operation->PickedItem = PickedItemIn;
	Operation->InventoryItemObj = InventoryItemObjIn;
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