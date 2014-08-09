// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGameEditor.h"

#include "ARItemPickerWidget.h"


void SARItemPickerWidget::Construct(const FArguments& InArgs)
{
	ItemContainer = InArgs._ItemContainer;


	if (ItemContainer && ItemContainer->ItemData.Num() > 0)
	{
		for (UARItemsData* data : ItemContainer->ItemData)
		{
			if (data)
			{
				for (FItemEntry& entry : data->EditEntries)
				{
					ItemNames.Add(MakeShareable(new FString(entry.ItemDataInfo.ItemName)));
				}
			}
		}
	}

	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SListView<TSharedPtr<FString>>)
			.ListItemsSource(&ItemNames)
			.OnGenerateRow(this, &SARItemPickerWidget::MakeListItems)
		]
	];
}


TSharedRef<ITableRow> SARItemPickerWidget::MakeListItems(TSharedPtr<FString> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow< TSharedPtr<FString> >, OwnerTable)
		[
			SNew(STextBlock).Text(*Item.Get())
		];
}