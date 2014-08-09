// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGameEditor.h"
#include "ARItemPickerWidget.h"
#include "ARItemPickerContainerCustomization.h"


void FARItemPickerContainerCustomization::CustomizeHeader(TSharedRef<class IPropertyHandle> StructPropertyHandle,
	class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	StructPropandle = StructPropertyHandle;
	HeaderRow
		.NameContent()
		[
			StructPropandle->CreatePropertyNameWidget()
		]
	.ValueContent()
	.MaxDesiredWidth(500)
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SNew(SButton)
			.Text(FText::FromString("Add Item"))
			.OnClicked(this, &FARItemPickerContainerCustomization::OnAddItemClicked)
		]
	];
}


FReply FARItemPickerContainerCustomization::OnAddItemClicked()
{
	TArray<UObject*> OuterObjects;
	StructPropandle->GetOuterObjects(OuterObjects);

	FARItemPickerContainer* itemCont = nullptr;
	UStructProperty* structProp = CastChecked<UStructProperty>(StructPropandle->GetProperty());
	if (structProp)
	{
		for (UObject* obj : OuterObjects)
		{
			itemCont = structProp->ContainerPtrToValuePtr<FARItemPickerContainer>(obj);
		}
	}

	ItemPickerWindow = SNew(SWindow)
		.Title(FText::FromString("Pick Items"))
		.ClientSize(FVector2D(600, 300))
		[
			SNew(SARItemPickerWidget)
			.ItemContainer(itemCont)
		];
	ItemPickerWindow->SetOnWindowDeactivated(FOnWindowDeactivated::CreateRaw(this, &FARItemPickerContainerCustomization::OnItemPickerWindowtWindowDeactivate));
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


void FARItemPickerContainerCustomization::OnItemPickerWindowtWindowDeactivate()
{
	if (ItemPickerWindow.IsValid())
	{
		ItemPickerWindow->RequestDestroyWindow();
	}
}