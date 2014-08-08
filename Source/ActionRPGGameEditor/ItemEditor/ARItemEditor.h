// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "ActionRPGGameEditor.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "Toolkits/AssetEditorManager.h"


DECLARE_DELEGATE_TwoParams(FOnItemSelected, FName, int32);

class SAddItemOptions : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SAddItemOptions)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<UARItemsData>, ItemAsset)
		SLATE_ARGUMENT(TWeakPtr<SWindow>, OwningWindow)
		SLATE_ARGUMENT(TWeakPtr<class SItemListWidget>, ItemList)
	SLATE_END_ARGS()
public:
	void Construct(const FArguments& InArgs);

	void SetItemName(const FText& TextIn, ETextCommit::Type CommitType);
	FReply OnAddItem();

	FReply OnCancel();
	TWeakObjectPtr<UARItemsData> ItemAsset;
	TWeakPtr<SWindow> OwningWindow;
	TWeakPtr<class SItemListWidget> ItemList;
	FString ItemName;

};

class SItemListWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SItemListWidget)
	{}
	SLATE_EVENT(FOnItemSelected, OnItemSelected)
		SLATE_ARGUMENT(UARItemsData*, ItemAsset)
	SLATE_END_ARGS()
public:
	void Construct(const FArguments& InArgs);

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	UARItemsData* ItemAsset;

	/*
		Because there is no direct way to use TMap with Slate (at least I don't know any).
		We will perform hard copy of some properties to array, each time TMap is updated.
		Unfortunetly there to just append new entries, we will clear array and do full copy over.

		this is very bad, but should work for now.
	*/
	void SyncItemList();

	int32 GetItemIndex();
	int32 CurrentIndex;
	FName CurrentKey;

	FOnItemSelected OnItemSelected;

	

	TArray<TSharedPtr<FName>> ItemList;

	TSharedPtr<class FEdGraphSchemaAction_RPGItemEntry> GetSelectedEntryInternal() const;

	TSharedPtr<SGraphActionMenu> GraphActionMenu;
	TSharedRef<SWidget> HandleCreateWidgetForAction(FCreateWidgetForActionData* const InCreateData);
	void HandleCollectAllActions(FGraphActionListBuilderBase& GraphActionListBuilder);
	void HandleActionSelected(const TArray< TSharedPtr<FEdGraphSchemaAction> >& SelectedActions) const;

	TSharedRef<ITableRow> MakeTileViewWidget(TSharedPtr<FName> AssetItem, const TSharedRef<STableViewBase>& OwnerTable);
};

class FARItemEditor : public FAssetEditorToolkit
{
public:
	//IToolkit Interface
	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	//End IToolkit Interface

	// FAssetEditorToolkit
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FText GetToolkitName() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FString GetDocumentationLink() const override;
	// End of FAssetEditorToolkit

	~FARItemEditor();
	void InitiItemEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UARItemsData* ItemIn);

	TSharedRef<SDockTab> SpawnTab_Item(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_Details(const FSpawnTabArgs& Args);
protected:

	TSharedRef<SWidget> SetItemDetails();

	int32 GetCurrentIndex();

	UARItemsData* EditedItemAsset;

	FOnItemSelected OnItemSelected;

	TSharedPtr< class IDetailsView > ItemDetailsView;

	//TSharedRef<class FPropertyChangeListener> PropertyChange;
	FReply CreateNewItem();
	/* Edit Widgets */
	TSharedPtr<SItemListWidget> ItemEntry;

	int32 CurrentIndex;
	void SaveEditedPRoperties(const FPropertyChangedEvent& EventIn);

	void HandleBlackboardEntrySelected(FName BlackboardEntry, int32 ItemIndex);
	
private:
	/**	The tab id for the curve table tab */
	static const FName ItemEditorTabId;
	static const FName ItemEditorTabDetails;
};
