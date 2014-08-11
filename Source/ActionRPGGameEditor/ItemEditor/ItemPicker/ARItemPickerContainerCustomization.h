// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "ARItemPickerWidget.h"
#include "PropertyEditing.h"
#include "PropertyCustomizationHelpers.h"

DECLARE_DELEGATE_OneParam(FOnGetItemContainer, FARItemPickerContainer*)

class SItemContainerPalleteItem : public SGraphPaletteItem
{
	SLATE_BEGIN_ARGS(SItemContainerPalleteItem) {}


	SLATE_END_ARGS()
public:
	void Construct(const FArguments& InArgs, FCreateWidgetForActionData* const InCreateData);

};

class SARItemContainerList : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SARItemContainerList)
	{}
		SLATE_ATTRIBUTE(FARItemPickerContainer*, ItemPickupCont)
			SLATE_EVENT(FOnItemDataSelected, OnGetContainer)
	SLATE_END_ARGS()

public:
	~SARItemContainerList();
	void Construct(const FArguments& InArgs);

	TSharedRef<SWidget> HandleCreateWidgetForAction(FCreateWidgetForActionData* const InCreateData);
	void HandleCollectAllActions(FGraphActionListBuilderBase& GraphActionListBuilder);
	void HandleActionSelected(const TArray< TSharedPtr<FEdGraphSchemaAction> >& SelectedActions) const;
	TSharedPtr<class FEdGraphSchemaAction_ItemContainerEntry> GetSelectedEntryInternal() const;

	TSharedPtr<SGraphActionMenu> GraphActionMenu;

	FOnItemDataSelected OnGetContainer;
	void GetContainer(const FARItemPickerContainer* ContainerIn);


	TAttribute<FARItemPickerContainer*> ItemPickupCont;

};



class FARItemPickerContainerCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShareable(new FARItemPickerContainerCustomization());
	}

	~FARItemPickerContainerCustomization();

	/** Overridden to show an edit button to launch the gameplay tag editor */
	virtual void CustomizeHeader(TSharedRef<class IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

	/** Overridden to do nothing */
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> InStructPropertyHandle, class IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override {}

private:
	FReply OnAddItemClicked();

	//Picker Window
	void OnItemPickerWindowtWindowDeactivate();
	void OnItemPickerWindowtWindowClosed(const TSharedRef<SWindow>& WindowIn);
	TSharedPtr<SWindow> ItemPickerWindow;

	void RefreshItemList();
	TSharedRef<SWidget> CurrentItemsWidget();

	//Asset picker
	void OnAssetSelected(const class FAssetData& PickedAsset);
	void OnGetAllowedClasses(TArray<const UClass*>& AllowedClasses);

	//Currently selected item data

	//ItemPicker;
	TSharedPtr<SARItemPickerWidget> ItemPickerWidget;


	//Container Selector
	FOnItemDataSelected OnGetContainer;
	TAttribute<FARItemPickupCont*> OnItemContSelected;

	TArray<TSharedPtr<FString>> ItemList;
	FARItemPickupCont* GetCurrentItemCont() const;
	FARItemPickupCont* CurrentItemCont;
	void SetCurrentItemCont(FARItemPickupCont* ContIn);

	TWeakObjectPtr<UObject> EditedObject;

	FOnItemDataSelected OnItemSelected;

	TSharedPtr<SListView<TSharedPtr<FString>>> ItemListWidget;

	TSharedRef<ITableRow> MakeItemListRow(TSharedPtr<FString> Item, const TSharedRef<STableViewBase>& OwnerTable);

	TSharedPtr<IPropertyHandle> StructPropHandle;

	FARItemPickerContainer* GetCurrentlyEditedStruct() const;
	TAttribute<FARItemPickerContainer*> OnCurrentlyEditedStruct;
	FARItemPickerContainer* CurrentlyEditedStruct;

	

	FOnGetAllowedClasses OnGetClasses;
	FOnAssetSelected OnAssetSlect;
	FOnShouldFilterAsset OnShouldFilterAsset;
	FSimpleDelegate OnClose;
};


