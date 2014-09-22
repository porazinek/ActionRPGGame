// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGameEditor.h"

#include "ARItemPickerWidget.h"

class FEdGraphSchemaAction_ItemPickerEntry : public FEdGraphSchemaAction_Dummy
{
public:
	static FName StaticGetTypeId() { static FName Type = TEXT("FEdGraphSchemaAction_ItemPickerEntry"); return Type; }
	virtual FName GetTypeId() const override { return StaticGetTypeId(); }

	FEdGraphSchemaAction_ItemPickerEntry(FString ItemNameIn, int32 ItemIndexIn)
		//: ItemData(ItemDataIn)
		: ItemName(ItemNameIn)
		, ItemIndex(ItemIndexIn)
	{
		MenuDescription = FText::FromString(ItemName);
		TooltipDescription = FString(ItemName);
	}
	FString ItemName;
	int32 ItemIndex;
};

void SItemPickerPallete::Construct(const FArguments& InArgs, FCreateWidgetForActionData* const InCreateData)
{
	check(InCreateData);
	check(InCreateData->Action.IsValid());

	TSharedPtr<FEdGraphSchemaAction> GraphAction = InCreateData->Action;
	check(GraphAction->GetTypeId() == FEdGraphSchemaAction_ItemPickerEntry::StaticGetTypeId());
	
	TSharedPtr<FEdGraphSchemaAction_ItemPickerEntry> ItemPickerEntryAction = StaticCastSharedPtr<FEdGraphSchemaAction_ItemPickerEntry>(GraphAction);

	ActionPtr = InCreateData->Action;

	this->ChildSlot
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			[
				SNew(STextBlock)
				.Text(this, &SItemPickerPallete::GetDisplayText)
			]
		];
}


void SARItemPickerWidget::Construct(const FArguments& InArgs)
{
	ItemPickupCont = InArgs._ItemPickupCont;
	EditedObject = InArgs._EditedObject;
	OnItemAdded = InArgs._OnItemAdded;
	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		[
			SAssignNew(GraphActionMenu, SGraphActionMenu, false)
			.OnCreateWidgetForAction(this, &SARItemPickerWidget::HandleCreateWidgetForAction)
			.OnCollectAllActions(this, &SARItemPickerWidget::HandleCollectAllActions)
			.OnActionSelected(this, &SARItemPickerWidget::HandleActionSelected)
		]
	];
}
SARItemPickerWidget::~SARItemPickerWidget()
{
	if (GraphActionMenu.IsValid())
		GraphActionMenu.Reset();
}
TSharedRef<SWidget> SARItemPickerWidget::HandleCreateWidgetForAction(FCreateWidgetForActionData* const InCreateData)
{
	return SNew(SItemPickerPallete, InCreateData);
}
void SARItemPickerWidget::HandleCollectAllActions(FGraphActionListBuilderBase& GraphActionListBuilder)
{
	if (ItemPickupCont.Get())
	{
		if (ItemPickupCont.Get()->ItemData && ItemPickupCont.Get()->ItemData->EditEntries.Num() > 0)
		{
			for (FItemEntry& entry : ItemPickupCont.Get()->ItemData->EditEntries)
			{
				GraphActionListBuilder.AddAction(MakeShareable(new FEdGraphSchemaAction_ItemPickerEntry(entry.ItemDataInfo.ItemName, entry.Index)));
			}
		}
	}
}
void SARItemPickerWidget::HandleActionSelected(const TArray< TSharedPtr<FEdGraphSchemaAction> >& SelectedActions) const
{
	if (SelectedActions.Num() > 0)
	{
		check(SelectedActions[0]->GetTypeId() == FEdGraphSchemaAction_ItemPickerEntry::StaticGetTypeId());
		TSharedPtr<FEdGraphSchemaAction_ItemPickerEntry> ItemEntry = StaticCastSharedPtr<FEdGraphSchemaAction_ItemPickerEntry>(SelectedActions[0]);
		if (ItemPickupCont.Get())
		{
			ItemPickupCont.Get()->ItemsList.Add(ItemEntry->ItemIndex);
			OnItemAdded.ExecuteIfBound();
			EditedObject->MarkPackageDirty();
		}
	}
}

TSharedPtr<class FEdGraphSchemaAction_ItemPickerEntry> SARItemPickerWidget::GetSelectedEntryInternal() const
{
	TArray< TSharedPtr<FEdGraphSchemaAction> > SelectedActions;
	GraphActionMenu->GetSelectedActions(SelectedActions);

	if (SelectedActions.Num() > 0)
	{
		check(SelectedActions[0]->GetTypeId() == FEdGraphSchemaAction_ItemPickerEntry::StaticGetTypeId());
		return StaticCastSharedPtr<FEdGraphSchemaAction_ItemPickerEntry>(SelectedActions[0]);
	}

	return TSharedPtr<FEdGraphSchemaAction_ItemPickerEntry>();
}

TSharedRef<ITableRow> SARItemPickerWidget::MakeListItems(TSharedPtr<FString> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow< TSharedPtr<FString> >, OwnerTable)
		[
			SNew(STextBlock).Text(*Item.Get())
		];
}