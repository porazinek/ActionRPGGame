// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "ActionRPGGameEditor.h"
#include "SlateCore.h"
#include "Slate.h"

DECLARE_DELEGATE_OneParam(FOnItemDataSelected, FARItemPickupCont*)
DECLARE_DELEGATE(FOnContainerSelected)
DECLARE_DELEGATE(FOnItemAdded)

class SItemPickerPallete : public SGraphPaletteItem
{
	SLATE_BEGIN_ARGS(SItemPickerPallete) {}


	SLATE_END_ARGS()
public:
	void Construct(const FArguments& InArgs, FCreateWidgetForActionData* const InCreateData);

};

class SARItemPickerWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SARItemPickerWidget)
	{}
		SLATE_ATTRIBUTE(FARItemPickupCont*, ItemPickupCont)
		SLATE_ARGUMENT(TWeakObjectPtr<UObject>, EditedObject)
		SLATE_EVENT(FOnItemAdded, OnItemAdded)
		SLATE_EVENT(FOnItemDataSelected, OnItemSelected)
		SLATE_EVENT(FOnContainerSelected, OnContainerSelected)
	SLATE_END_ARGS()

public:
	~SARItemPickerWidget();
	void Construct(const FArguments& InArgs);

	TSharedRef<SWidget> HandleCreateWidgetForAction(FCreateWidgetForActionData* const InCreateData);
	void HandleCollectAllActions(FGraphActionListBuilderBase& GraphActionListBuilder);
	void HandleActionSelected(const TArray< TSharedPtr<FEdGraphSchemaAction> >& SelectedActions) const;
	TSharedPtr<class FEdGraphSchemaAction_ItemPickerEntry> GetSelectedEntryInternal() const;
	TSharedPtr<SGraphActionMenu> GraphActionMenu;

	TArray<TSharedPtr<FString>> ItemNames;

	TSharedRef<ITableRow> MakeListItems(TSharedPtr<FString> Item, const TSharedRef<STableViewBase>& OwnerTable);

	TWeakObjectPtr<UObject> EditedObject;
	TAttribute<FARItemPickupCont*> ItemPickupCont;
	FOnItemAdded OnItemAdded;
	FOnItemDataSelected OnItemSelected;
	FOnContainerSelected OnContainerSelected;
};


