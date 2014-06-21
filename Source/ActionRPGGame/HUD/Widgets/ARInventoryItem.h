// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../ARHUD.h"

#include "../../Types/ARStructTypes.h"

#include "SlateGameResources.h"
#include "Slate.h"

class FMyUIResources
{
public:
	FMyUIResources() : m_Path("/Game/UI"), MyUIResources(NULL) { };

	/*Loads resources and registers them with Slate*/
	/*Do this before trying to use resources*/
	void Initialize();

	/*cleanup*/
	/*Do this when shutting down game module*/
	void Shutdown();

	/*reloads textures used by slate renderer*/
	/*Does nothing at the moment*/
	void ReloadTextures();

	/*Give caller a pointer to our FSlateGameResources*/
	TSharedPtr<FSlateGameResources> GetSlateGameResources();

protected:
	/*Creates resources*/
	TSharedRef<class FSlateGameResources> Create();

	/*Defined in Cpp file, change as needed*/
	const FString m_Path;

	/*Poitner to game resources, Initialize() before using*/
	TSharedPtr<FSlateGameResources> MyUIResources;
};

class SARInventoryItem : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SARInventoryItem)
	{}
	SLATE_EVENT(FPointerEventHandler, OnMouseEnter)

		SLATE_EVENT(FPointerEventHandler, OnMouseLeave)

		SLATE_EVENT(FPointerEventHandler, OnMouseButtonDown)

		SLATE_EVENT(FPointerEventHandler, OnDragEnter)

		SLATE_ARGUMENT(TArray<TSharedPtr<FARItemInfo>>, Inventory)

		SLATE_ARGUMENT(TWeakObjectPtr<class AARHUD>, OwnerHUD)

		SLATE_ARGUMENT(TWeakObjectPtr<class AARPlayerController>, MyPC)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) OVERRIDE;

	virtual void OnDragEnter(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) OVERRIDE;
	virtual void OnDragLeave(const FDragDropEvent& DragDropEvent) OVERRIDE;
	virtual FReply OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) OVERRIDE;
	virtual FReply OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) OVERRIDE;
	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) OVERRIDE;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) OVERRIDE;
	virtual FReply OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) OVERRIDE;

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) OVERRIDE;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) OVERRIDE;
private:
	TSharedPtr<FSlateGameResources> MyUIResources;
	FSlateColor ImageColor;
	FSlateColor GetColor() const;
	const FSlateBrush* Icon;
	const FSlateBrush* GetImage() const;

	TArray<TSharedPtr<FARItemInfo>> Inventory;
	TWeakObjectPtr<class AARHUD> OwnerHUD;
	TWeakObjectPtr<class AARPlayerController> MyPC;

	TSharedPtr<STileView<TSharedPtr<FARItemInfo>>> TileView;
	TSharedRef<ITableRow> MakeTileViewWidget(TSharedPtr<FARItemInfo> AssetItem, const TSharedRef<STableViewBase>& OwnerTable);
	FReply Drag(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

	FText TextTest;
	FText GetText() const;
	TArray<TSharedPtr<FARItemInfo>> ClonedInventory;
	void SyncInventory();
	FText GetTextFromInv() const;


	TSharedPtr<SGridPanel> ActionGrid;
};



