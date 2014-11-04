// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../ARHUD.h"

#include "../../Types/ARStructTypes.h"

#include "SlateBasics.h"



class SARButtonWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SARButtonWidget)
	{}
	/*See private declaration of OwnerHUD below.*/
		SLATE_ARGUMENT(TWeakObjectPtr<class AARPlayerController>, MyPC)
		SLATE_ARGUMENT(TWeakObjectPtr<class AARCharacter>, Character)
		//SLATE_ARGUMENT(TArray<FARItemInfo>, Items)

	SLATE_END_ARGS()

public:
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/////Needed for every widget
	/////Builds this widget and any of it's children
	void Construct(const FArguments& InArgs);
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	~SARButtonWidget();

private:
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/////Pointer to our parent HUD
	/////To make sure HUD's lifetime is controlled elsewhere, use "weak" ptr.
	/////HUD has "strong" pointer to Widget,
	/////circular ownership would prevent/break self-destruction of hud/widget (cause memory leak).
	TWeakObjectPtr<class AARHUD> OwnerHUD;
	TWeakObjectPtr<class AARPlayerController> MyPC;
	TWeakObjectPtr<class AARCharacter> Character;

	FReply DoNothing(const FGeometry& Geometry, const FPointerEvent& MouseEvent);

	void EmptyFunction(FGeometry const& MyGeometry, FDragDropEvent const& DragDropEvent);


	TArray<TSharedPtr<FARItemInfo>> ClonedInventory;
	void SyncInventory();


	TArray<FARItemInfo> Items;

	TArray<FARItemInfo> GetItems();

	TArray<TSharedPtr<FARItemInfo>> GetInventory();

	TSharedPtr<SGridPanel> ActionGrid;
	TSharedPtr<class SARInventoryItem> InvItem;

};



