// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../ARHUD.h"

#include "../../Types/ARStructTypes.h"
#include "../../Types/AREnumTypes.h"
#include "SlateBasics.h"

class SARActionItemWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SARActionItemWidget)
	{}
	/*See private declaration of OwnerHUD below.*/
	SLATE_ARGUMENT(TWeakObjectPtr<class AARHUD>, OwnerHUD)

		SLATE_ARGUMENT(TWeakObjectPtr<class AARPlayerController>, MyPC)

		SLATE_ARGUMENT(TWeakObjectPtr<class UARAbilityComponent>, AbilityComp)

		SLATE_ARGUMENT(TSharedPtr<FActionSlotInfo>, CurrentAbility)

		SLATE_ARGUMENT(EARAbilitySlot, SlotType)
	SLATE_END_ARGS()

public:
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/////Needed for every widget
	/////Builds this widget and any of it's children
	void Construct(const FArguments& InArgs);
	//Destruct -;-
	~SARActionItemWidget();
	//Mouse Input
	virtual void OnDragEnter(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;
	virtual void OnDragLeave(const FDragDropEvent& DragDropEvent) override;
	virtual FReply OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;
	virtual FReply OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;
	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;
	virtual FReply OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;


private:
	TWeakObjectPtr<class AARHUD> OwnerHUD;

	TWeakObjectPtr<class AARPlayerController> MyPC;

	//Ability currently equiped. Synced with server.
	TSharedPtr<FActionSlotInfo> CurrentAbility;

	EARAbilitySlot SlotType;

	TSharedPtr<FActionSlotInfo> ActionItem;

	FARAbilityData* ActionData;

	//cosmetics only.
	TWeakObjectPtr<class AARAbility> Ability;

	TWeakObjectPtr<class UARAbilityComponent> AbilityComp;



	void DrawAbility();

	FText GetCurrentCooldown() const;

	const FSlateBrush* GetAbilityIcon() const;
};

class FAbilityDragDrop : public FDragDropOperation
{
	FAbilityDragDrop()
	{}
public:
	DRAG_DROP_OPERATOR_TYPE(FAbilityDragDrop, FDragDropOperation)
public:

	static TSharedRef<FAbilityDragDrop> New(TSharedPtr<FActionSlotInfo> PickedItemIn, TSharedPtr<SARActionItemWidget> LastAbilitySlotIm);

	virtual TSharedPtr<SWidget> GetDefaultDecorator() const override;
	
	FARAbilityData* ActionData;

	TSharedPtr<FActionSlotInfo> PickedAbility;
	TSharedPtr<SARActionItemWidget> LastAbilitySlot;
private:
	const FSlateBrush* GetDecoratorIcon() const;
};


