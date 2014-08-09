// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "PropertyEditing.h"
#include "PropertyCustomizationHelpers.h"

class FARItemPickerContainerCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShareable(new FARItemPickerContainerCustomization);
	}

	/** Overridden to show an edit button to launch the gameplay tag editor */
	virtual void CustomizeHeader(TSharedRef<class IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

	/** Overridden to do nothing */
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> InStructPropertyHandle, class IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override {}


	FReply OnAddItemClicked();

	void OnItemPickerWindowtWindowDeactivate();

	TSharedPtr<IPropertyHandle> StructPropandle;

	TSharedPtr<FARItemPickerContainer> CurrentlyEditedStruct;

	TSharedPtr<SWindow> ItemPickerWindow;
};


