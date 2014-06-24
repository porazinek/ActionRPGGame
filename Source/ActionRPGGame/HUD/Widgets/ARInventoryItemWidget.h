// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../ARHUD.h"

#include "../../Types/ARStructTypes.h"
#include "../../Types/AREnumTypes.h"
#include "Slate.h"

//this class begs for refactor.
//move some of drag and drop functionality higher in hierarchy
//as we will reuse it quite a bit.
class SARInventoryItemWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SARInventoryItemWidget)
	{}
	/*See private declaration of OwnerHUD below.*/
		SLATE_ARGUMENT(TWeakObjectPtr<class AARPlayerController>, PlayerController)
		SLATE_ARGUMENT(TWeakObjectPtr<class AARCharacter>, Character)
		SLATE_ARGUMENT(TSharedPtr<FARItemInfo>, InventoryItem)

		SLATE_ARGUMENT(TSharedPtr<FInventorySlot>, InventoryItemObj)

		SLATE_ARGUMENT(TEnumAsByte<EItemSlot>, SlotType)
		SLATE_ARGUMENT(TEnumAsByte<EEquipmentSlot::Type>, EquipmentSlot)
		SLATE_ARGUMENT(TWeakObjectPtr<class AARItem>, ItemInSlot)

		SLATE_ARGUMENT(FText, SlotName) //temp it should really display icon.
	SLATE_END_ARGS()
public:
	void Construct(const FArguments& InArgs);
	~SARInventoryItemWidget();
	//Mouse input
	virtual void OnDragEnter(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;
	virtual void OnDragLeave(const FDragDropEvent& DragDropEvent) override;
	virtual FReply OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;
	virtual FReply OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;
	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;
	virtual FReply OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

protected:

public:
	TSharedPtr<FARItemInfo> InventoryItem;
	TWeakObjectPtr<class AARPlayerController> PlayerController;
	TWeakObjectPtr<class AARCharacter> Character;
	TEnumAsByte<EItemSlot> SlotType;
	TEnumAsByte<EEquipmentSlot::Type> EquipmentSlot;
	//item equiped in current slot. Coressponds to pointers in EquipmentComponent.
	TWeakObjectPtr<class AARItem> ItemInSlot;

	TSharedPtr<FInventorySlot> InventoryItemObj;

	FText SlotName;

	const FSlateBrush* GetImage() const;
	FText GetItemText() const;
	FSlateColor GetTextColor() const;

	FSlateColor TextColor;

};

class FInventoryDragDrop : public FDragDropOperation
{
	FInventoryDragDrop()
	{}
public:
	DRAG_DROP_OPERATOR_TYPE(FInventoryDragDrop, FDragDropOperation)

	//FInventoryDragDrop()
	//{}
	
	static TSharedRef<FInventoryDragDrop> New(TSharedPtr<FInventorySlot> PickedItemIn, TWeakObjectPtr<class AARItem> InventoryItemObjIn, TSharedPtr<SARInventoryItemWidget> LastItemSlotIn);

	virtual TSharedPtr<SWidget> GetDefaultDecorator() const override;

	TSharedPtr<FInventorySlot> PickedItem;
	TWeakObjectPtr<class AARItem> InventoryItemObj;
	TSharedPtr<SARInventoryItemWidget> LastItemSlot;

private:
	const FSlateBrush* GetDecoratorIcon() const;
};

