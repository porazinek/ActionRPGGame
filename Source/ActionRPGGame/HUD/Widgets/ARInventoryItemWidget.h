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
		/*
			Controller which owns Inventory
		*/
		SLATE_ARGUMENT(TWeakObjectPtr<class AARPlayerController>, PlayerController)
		
		SLATE_ARGUMENT(TWeakObjectPtr<class UARInventoryComponent>, Inventory)

		SLATE_ARGUMENT(TWeakObjectPtr<class UAREquipmentComponent>, Equipment)

		SLATE_ARGUMENT(TWeakObjectPtr<class AARCharacter>, Character)

		SLATE_ARGUMENT(TWeakObjectPtr<class AARItemPickup>, LootedObject)

		/*
			Information about current item slot in inventory.
		*/
		SLATE_ARGUMENT(TSharedPtr<FARDragDropInfo>, InventoryItem)

		/*
			Type of slot used by item in this inventory slot.
			I forgot why I needed it ;o.
		*/
		SLATE_ARGUMENT(TEnumAsByte<EItemSlot>, SlotType)
		/*
			Slot to which item will be equiped. 
		*/
		//SLATE_ARGUMENT(TEnumAsByte<EEquipmentSlot::Type>, EquipmentSlot)
		/*
			To display something.
		*/
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
	/*
		Controller which owns Inventory
	*/
	TWeakObjectPtr<class AARPlayerController> PlayerController;

	TWeakObjectPtr<class UARInventoryComponent> Inventory;

	TWeakObjectPtr<class UAREquipmentComponent> Equipment;

	TWeakObjectPtr<class AARCharacter> Character;

	TWeakObjectPtr<class AARItemPickup> LootedObject;
	/*
		Information about current item slot in inventory.
	*/
	TSharedPtr<FARDragDropInfo> InventoryItem;

	/*
		Type of slot used by item in this inventory slot.
		I forgot why I needed it ;o.
	*/
	TEnumAsByte<EItemSlot> SlotType;
	/*
		Slot to which item will be equiped.
	*/
	//TEnumAsByte<EEquipmentSlot::Type> EquipmentSlot;

	FARItemInfo* ItemInThisSlot;
	
	FText SlotName;

	const FSlateBrush* GetImage() const;
	FText GetItemText() const;
	FSlateColor GetTextColor() const;

	FSlateColor TextColor;

private:
	void SpawnItem();

};

class FInventoryDragDrop : public FDragDropOperation
{
	FInventoryDragDrop()
	{}
public:
	DRAG_DROP_OPERATOR_TYPE(FInventoryDragDrop, FDragDropOperation)

	//FInventoryDragDrop()
	//{}
	
	static TSharedRef<FInventoryDragDrop> New(TSharedPtr<FARDragDropInfo> PickedItemIn, TSharedPtr<SARInventoryItemWidget> LastItemSlotIn);

	virtual TSharedPtr<SWidget> GetDefaultDecorator() const override;

	FARItemInfo* ItemInThisSlot;
	TSharedPtr<FARDragDropInfo> PickedItem;
	TSharedPtr<SARInventoryItemWidget> LastItemSlot;

private:
	const FSlateBrush* GetDecoratorIcon() const;
};

