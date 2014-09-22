// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGameEditor.h"

#include "ARItemPickerContainerCustomization.h"

#include "SComboBox.h"
#include "Editor/PropertyEditor/Public/PropertyCustomizationHelpers.h"

#include "Editor/ContentBrowser/Public/ContentBrowserModule.h"
#include "Editor/PropertyEditor/Private/Presentation/PropertyEditor/PropertyEditor.h"
#include "Runtime/AssetRegistry/Public/AssetData.h"
#include "Editor/ClassViewer/Public/ClassViewerModule.h"
#include "Editor/ClassViewer/Public/ClassViewerFilter.h"
#include "Editor/PropertyEditor/Private/UserInterface/PropertyEditor/SPropertyAssetPicker.h"
#include "Editor/ActorPickerMode/Public/ActorPickerMode.h"
#include "Editor/PropertyEditor/Private/PropertyHandleImpl.h"


class FEdGraphSchemaAction_ItemContainerEntry : public FEdGraphSchemaAction_Dummy
{
public: //this will need chagne to FName
	static FName StaticGetTypeId() { static FName Type = TEXT("FEdGraphSchemaAction_ItemContainerEntry"); return Type; }
	virtual FName GetTypeId() const override { return StaticGetTypeId(); }

	FEdGraphSchemaAction_ItemContainerEntry(FString ItemNameIn, FARItemPickupCont* ItemContIn)
		//: ItemData(ItemDataIn)
		: ItemName(ItemNameIn)
		, ItemCont(ItemContIn)
	{
		MenuDescription = FText::FromString(ItemName);
		TooltipDescription = FString(ItemName);
	}
	FString ItemName;
	FARItemPickupCont* ItemCont;
};

void SItemContainerPalleteItem::Construct(const FArguments& InArgs, FCreateWidgetForActionData* const InCreateData)
{
	check(InCreateData);
	check(InCreateData->Action.IsValid());

	TSharedPtr<FEdGraphSchemaAction> GraphAction = InCreateData->Action;
	check(GraphAction->GetTypeId() == FEdGraphSchemaAction_ItemContainerEntry::StaticGetTypeId());

	TSharedPtr<FEdGraphSchemaAction_ItemContainerEntry> ItemPickerEntryAction = StaticCastSharedPtr<FEdGraphSchemaAction_ItemContainerEntry>(GraphAction);

	ActionPtr = InCreateData->Action;

	this->ChildSlot
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			[
				SNew(STextBlock)
				.Text(this, &SItemContainerPalleteItem::GetDisplayText)
			]
		];
}

void SARItemContainerList::Construct(const FArguments& InArgs)
{
	ItemPickupCont = InArgs._ItemPickupCont;
	OnGetContainer = InArgs._OnGetContainer;

	ChildSlot
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				SAssignNew(GraphActionMenu, SGraphActionMenu, false)
				.OnCreateWidgetForAction(this, &SARItemContainerList::HandleCreateWidgetForAction)
				.OnCollectAllActions(this, &SARItemContainerList::HandleCollectAllActions)
				.OnActionSelected(this, &SARItemContainerList::HandleActionSelected)
			]
		];
}

void SARItemContainerList::GetContainer(const FARItemPickerContainer* ContainerIn)
{
	if (GraphActionMenu.IsValid())
	{
		GraphActionMenu->RefreshAllActions(false, true);
	}
}
SARItemContainerList::~SARItemContainerList()
{
}
TSharedRef<SWidget> SARItemContainerList::HandleCreateWidgetForAction(FCreateWidgetForActionData* const InCreateData)
{
	return SNew(SItemContainerPalleteItem, InCreateData);
}
void SARItemContainerList::HandleCollectAllActions(FGraphActionListBuilderBase& GraphActionListBuilder)
{
	if (ItemPickupCont.Get() && ItemPickupCont.Get()->ListOfItems.Num() > 0)
	{
		for (FARItemPickupCont& entry : ItemPickupCont.Get()->ListOfItems)
		{
			GraphActionListBuilder.AddAction(MakeShareable(new FEdGraphSchemaAction_ItemContainerEntry(entry.ItemDataName, &entry)));
		}
	}
}
void SARItemContainerList::HandleActionSelected(const TArray< TSharedPtr<FEdGraphSchemaAction> >& SelectedActions) const
{
	if (SelectedActions.Num() > 0)
	{
		check(SelectedActions[0]->GetTypeId() == FEdGraphSchemaAction_ItemContainerEntry::StaticGetTypeId());
		TSharedPtr<FEdGraphSchemaAction_ItemContainerEntry> ItemEntry = StaticCastSharedPtr<FEdGraphSchemaAction_ItemContainerEntry>(SelectedActions[0]);
		if (ItemEntry->ItemCont)
		{
			OnGetContainer.ExecuteIfBound(ItemEntry->ItemCont);
		}
	}
}




class FEdGraphSchemaAction_SelectedItemContainerEntry : public FEdGraphSchemaAction_Dummy
{
public: //this will need chagne to FName
	static FName StaticGetTypeId() { static FName Type = TEXT("FEdGraphSchemaAction_SelectedItemContainerEntry"); return Type; }
	virtual FName GetTypeId() const override { return StaticGetTypeId(); }

	FEdGraphSchemaAction_SelectedItemContainerEntry(FString ItemNameIn, FARItemPickupCont* ItemContIn, int32 ItemIDIn)
		: ItemName(ItemNameIn)
		, ItemCont(ItemContIn)
		, ItemID(ItemIDIn)
	{
		MenuDescription = FText::FromString(ItemName);
		TooltipDescription = FString(ItemName);
	}
	int32 ItemID;
	FString ItemName;
	FARItemPickupCont* ItemCont;
};

void SSelectedItemContainerPalleteItem::Construct(const FArguments& InArgs, FCreateWidgetForActionData* const InCreateData)
{
	check(InCreateData);
	check(InCreateData->Action.IsValid());

	TSharedPtr<FEdGraphSchemaAction> GraphAction = InCreateData->Action;
	check(GraphAction->GetTypeId() == FEdGraphSchemaAction_SelectedItemContainerEntry::StaticGetTypeId());

	TSharedPtr<FEdGraphSchemaAction_SelectedItemContainerEntry> ItemPickerEntryAction = StaticCastSharedPtr<FEdGraphSchemaAction_SelectedItemContainerEntry>(GraphAction);

	ActionPtr = InCreateData->Action;

	this->ChildSlot
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			[
				SNew(STextBlock)
				.Text(this, &SSelectedItemContainerPalleteItem::GetDisplayText)
			]
		];
}
void SARSelectedItemContainerList::Construct(const FArguments& InArgs)
{
	ItemPickupCont = InArgs._ItemPickupCont;
	OnGetContainer = InArgs._OnGetContainer;
	ItemContainer = InArgs._ItemContainer;

	ChildSlot
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				SAssignNew(GraphActionMenu, SGraphActionMenu, false)
				.OnCreateWidgetForAction(this, &SARSelectedItemContainerList::HandleCreateWidgetForAction)
				.OnCollectAllActions(this, &SARSelectedItemContainerList::HandleCollectAllActions)
				.OnActionSelected(this, &SARSelectedItemContainerList::HandleActionSelected)
			]
		];
}

void SARSelectedItemContainerList::GetContainer(const FARItemPickerContainer* ContainerIn)
{
	if (GraphActionMenu.IsValid())
	{
		GraphActionMenu->RefreshAllActions(false, true);
	}
}
SARSelectedItemContainerList::~SARSelectedItemContainerList()
{
}
TSharedRef<SWidget> SARSelectedItemContainerList::HandleCreateWidgetForAction(FCreateWidgetForActionData* const InCreateData)
{
	return SNew(SSelectedItemContainerPalleteItem, InCreateData);
}
void SARSelectedItemContainerList::HandleCollectAllActions(FGraphActionListBuilderBase& GraphActionListBuilder)
{
	if (ItemContainer.Get() && ItemContainer.Get()->ItemsList.Num() > 0)
	{
		for (int32 itemIndex : ItemContainer.Get()->ItemsList)
		{
			FARItemInfo* tempItem = ItemContainer.Get()->ItemData->GetItemDataFromArrayPtr(itemIndex);
			if (tempItem)
				GraphActionListBuilder.AddAction(MakeShareable(new FEdGraphSchemaAction_SelectedItemContainerEntry(tempItem->ItemName, ItemContainer.Get(), itemIndex)));
		}
	}
}
void SARSelectedItemContainerList::HandleActionSelected(const TArray< TSharedPtr<FEdGraphSchemaAction> >& SelectedActions) const
{
	if (SelectedActions.Num() > 0)
	{
		check(SelectedActions[0]->GetTypeId() == FEdGraphSchemaAction_SelectedItemContainerEntry::StaticGetTypeId());
		TSharedPtr<FEdGraphSchemaAction_SelectedItemContainerEntry> ItemEntry = StaticCastSharedPtr<FEdGraphSchemaAction_SelectedItemContainerEntry>(SelectedActions[0]);
		//if (ItemEntry->ItemCont)
		//{
		//	OnGetContainer.ExecuteIfBound(ItemEntry->ItemCont);
		//}
	}
}





void FARItemPickerContainerCustomization::CustomizeHeader(TSharedRef<class IPropertyHandle> StructPropertyHandle,
class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	CurrentItemCont = nullptr;
	CurrentlyEditedStruct = nullptr;
	OnGetClasses.BindRaw(this, &FARItemPickerContainerCustomization::OnGetAllowedClasses);
	OnAssetSlect.BindRaw(this, &FARItemPickerContainerCustomization::OnAssetSelected);
	OnItemContSelected.BindRaw(this, &FARItemPickerContainerCustomization::GetCurrentItemCont);
	OnCurrentlyEditedStruct.BindRaw(this, &FARItemPickerContainerCustomization::GetCurrentlyEditedStruct);
	StructPropHandle = StructPropertyHandle;

	TArray<UObject*> OuterObjects;
	StructPropHandle->GetOuterObjects(OuterObjects);

	FARItemPickerContainer* itemCont = nullptr;
	UStructProperty* structProp = CastChecked<UStructProperty>(StructPropHandle->GetProperty());
	if (structProp)
	{
		for (UObject* obj : OuterObjects)
		{
			EditedObject = obj;
			CurrentlyEditedStruct = structProp->ContainerPtrToValuePtr<FARItemPickerContainer>(obj);
		}
	}
	RefreshItemList();
	HeaderRow
		.NameContent()
		[
			StructPropHandle->CreatePropertyNameWidget()
		]
	.ValueContent()
		.MaxDesiredWidth(500)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.MaxWidth(100)
				[
					SNew(SButton)
					.Text(FText::FromString("Add Item"))
					.OnClicked(this, &FARItemPickerContainerCustomization::OnAddItemClicked)
				]
				+ SHorizontalBox::Slot()
					[
						CurrentItemsWidget()
					]
			]
		];
}
FARItemPickerContainerCustomization::~FARItemPickerContainerCustomization()
{
	CurrentItemCont = nullptr;
	CurrentlyEditedStruct = nullptr;
	ItemList.Empty(ItemList.Num());
	if (ItemListWidget.IsValid())
		ItemListWidget.Reset();

	if (EditedObject.IsValid())
		EditedObject.Reset();

	if (StructPropHandle.IsValid())
		StructPropHandle.Reset();

	if (ItemPickerWidget.IsValid())
		ItemPickerWidget.Reset();

	if (ItemPickerWindow.IsValid())
		ItemPickerWindow.Reset();
}
void FARItemPickerContainerCustomization::RefreshItemList()
{
	if (!CurrentlyEditedStruct)
		return;

	if (ItemList.Num() > 0)
	{
		ItemList.Empty(ItemList.Num());
	}

	if (CurrentlyEditedStruct->ListOfItems.Num() > 0)
	{
		for (FARItemPickupCont& cont : CurrentlyEditedStruct->ListOfItems)
		{
			if (cont.ItemsList.Num() > 0)
			{
				for (int32 index : cont.ItemsList)
				{
					if (index != INDEX_NONE && cont.ItemData)
					{
						FItemEntry* tempItem = cont.ItemData->GetItemFromArrayPtr(index);

						ItemList.Add(MakeShareable(new FString(tempItem->ItemDataInfo.ItemName)));
					}
				}
			}
		}
	}
	if (ItemListWidget.IsValid())
		ItemListWidget->RequestListRefresh();
}

TSharedRef<SWidget> FARItemPickerContainerCustomization::CurrentItemsWidget()
{
	RefreshItemList();
	SAssignNew(ItemListWidget, SListView<TSharedPtr<FString>>)
		.ListItemsSource(&ItemList)
		.OnGenerateRow(this, &FARItemPickerContainerCustomization::MakeItemListRow);

	return ItemListWidget.ToSharedRef();
}

TSharedRef<ITableRow> FARItemPickerContainerCustomization::MakeItemListRow(TSharedPtr<FString> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow< TSharedPtr<FString> >, OwnerTable)
		[
			SNew(STextBlock).Text(*Item.Get())
		];
}

FReply FARItemPickerContainerCustomization::OnAddItemClicked()
{
	TArray<const UClass*> classes;
	classes.Add(UARItemsData::StaticClass());
	FAssetData InitialObject;
	ItemPickerWindow = SNew(SWindow)
		.Title(FText::FromString("Pick Items"))
		.ClientSize(FVector2D(600, 300))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.MaxHeight(30)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					[
						SNew(SComboButton)
						.ButtonContent()
						[
							SNew(STextBlock)
							.Text(FText::FromString("Pick Items Data"))
						]
						.MenuContent()
						[
							PropertyCustomizationHelpers::MakeAssetPickerWithMenu(InitialObject, true, &classes, OnShouldFilterAsset, OnAssetSlect, OnClose)
						]
					]
				]
				+ SHorizontalBox::Slot()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					[
						SNew(SComboButton)
						.ButtonContent()
						[
							SNew(STextBlock)
							.Text(FText::FromString("Pick current item Container"))
						]
						.MenuContent()
						[
							SAssignNew(ItemContainerList, SARItemContainerList)
							.ItemPickupCont(OnCurrentlyEditedStruct)
							.OnGetContainer(this, &FARItemPickerContainerCustomization::SetCurrentItemCont)
						]
					]
				]
			]
			+ SVerticalBox::Slot()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.MaxHeight(24)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SNew(STextBlock)
							.Text(FText::FromString("Pick Items Below"))
						]
					]
					+ SVerticalBox::Slot()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SAssignNew(ItemPickerWidget, SARItemPickerWidget)
							.ItemPickupCont(OnItemContSelected)
							.EditedObject(EditedObject)
							.OnItemAdded(this, &FARItemPickerContainerCustomization::OnItemAdded)
						]
					]
				]
				+ SHorizontalBox::Slot()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.MaxHeight(24)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SNew(STextBlock)
							.Text(FText::FromString("Remove Items Below"))
						]
					]
					+ SVerticalBox::Slot()
						.MaxHeight(24)
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							[
								SNew(SButton)
								.OnClicked(this, &FARItemPickerContainerCustomization::RemoveItemsAssetOnClicked)
								.Text(FText::FromString("Remove Items Asset"))
							]
						]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SAssignNew(SelectedItemsContainer, SARSelectedItemContainerList)
							.ItemContainer(OnItemContSelected)
						]
					]
	
				]
			]

		];


	ItemPickerWindow->SetOnWindowDeactivated(FOnWindowDeactivated::CreateRaw(this, &FARItemPickerContainerCustomization::OnItemPickerWindowtWindowDeactivate));
	ItemPickerWindow->SetOnWindowClosed(FOnWindowClosed::CreateRaw(this, &FARItemPickerContainerCustomization::OnItemPickerWindowtWindowClosed));

	IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>(TEXT("MainFrame"));

	if (MainFrameModule.GetParentWindow().IsValid())
	{
		FSlateApplication::Get().AddWindowAsNativeChild(ItemPickerWindow.ToSharedRef(), MainFrameModule.GetParentWindow().ToSharedRef());
	}
	else
	{
		FSlateApplication::Get().AddWindow(ItemPickerWindow.ToSharedRef());
	}

	return FReply::Unhandled();
}
void FARItemPickerContainerCustomization::OnItemAdded()
{
	SelectedItemsContainer->GraphActionMenu->RefreshAllActions(false, true);
}
FARItemPickerContainer* FARItemPickerContainerCustomization::GetCurrentlyEditedStruct() const
{
	return CurrentlyEditedStruct;
}
void FARItemPickerContainerCustomization::SetCurrentItemCont(FARItemPickupCont* ContIn)
{
	CurrentItemCont = ContIn;
	ItemPickerWidget->GraphActionMenu->RefreshAllActions(false, true);
//	ItemContainerList->GraphActionMenu->RefreshAllActions(false, true);
	if (CurrentItemCont)
		SelectedItemsContainer->GraphActionMenu->RefreshAllActions(false, true);
}
FARItemPickupCont* FARItemPickerContainerCustomization::GetCurrentItemCont() const
{
	if (!CurrentItemCont)
		return nullptr;

	return CurrentItemCont;
}

FReply FARItemPickerContainerCustomization::RemoveItemsAssetOnClicked()
{
	if (!CurrentItemCont || !CurrentlyEditedStruct)
		return FReply::Unhandled();

	CurrentlyEditedStruct->RemoveItemContainer(CurrentItemCont);
	CurrentItemCont = nullptr;
	ItemPickerWidget->GraphActionMenu->RefreshAllActions(false, true);
	ItemContainerList->GraphActionMenu->RefreshAllActions(false, true);
	SelectedItemsContainer->GraphActionMenu->RefreshAllActions(false, true);

	EditedObject->MarkPackageDirty();

	return FReply::Handled();
}

void FARItemPickerContainerCustomization::OnAssetSelected(const class FAssetData& PickedAsset)
{
	if (!CurrentlyEditedStruct)
		return;

	FARItemPickupCont newItemCont;

	UARItemsData* tempItem = Cast<UARItemsData>(PickedAsset.GetAsset());

	if (tempItem)
	{
		TArray<UObject*> OuterObjects;
		StructPropHandle->GetOuterObjects(OuterObjects);

		//GetChildHandle
		FARItemPickerContainer* itemCont = nullptr;
		UStructProperty* structProp = CastChecked<UStructProperty>(StructPropHandle->GetProperty());
		if (structProp)
		{
			for (UObject* obj : OuterObjects)
			{
				//	EditedObject = obj;
				itemCont = structProp->ContainerPtrToValuePtr<FARItemPickerContainer>(obj);
			}
		}
		if (itemCont)
		{
			int32 itemIdx = itemCont->ListOfItems.Num();
			newItemCont.ItemData = tempItem;
			newItemCont.ItemDataName = tempItem->GetName();
			//if (itemIdx > 0)
			//{
			//	itemCont->ListOfItems[itemIdx-1].ItemData = tempItem;
			//	itemCont->ListOfItems[itemIdx-1].ItemDataName = tempItem->GetName();
			//}
			itemCont->ListOfItems.AddUnique(newItemCont);
		}
	}
	ItemContainerList->GraphActionMenu->RefreshAllActions(false, true);
}

void FARItemPickerContainerCustomization::OnGetAllowedClasses(TArray<const UClass*>& AllowedClasses)
{
	TArray<const UClass*> classes;
	classes.Add(UARItemsData::StaticClass());
	AllowedClasses = classes;
}

void FARItemPickerContainerCustomization::OnItemPickerWindowtWindowDeactivate()
{
	//	if (ItemPickerWindow.IsValid())
	//	{
	//		ItemPickerWindow->RequestDestroyWindow();
	//	}

}

void FARItemPickerContainerCustomization::OnItemPickerWindowtWindowClosed(const TSharedRef<SWindow>& WindowIn)
{
	RefreshItemList();
	CurrentItemCont = nullptr;


	if (ItemPickerWidget.IsValid())
		ItemPickerWidget.Reset();
}