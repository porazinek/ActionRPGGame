// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "PropertyEditing.h"
#include "PropertyCustomizationHelpers.h"

DECLARE_DELEGATE_RetVal(int32, FOnGetCurrentItemIndex);

class FARItemAssetDetails : public IDetailCustomization
{
public:
	FARItemAssetDetails(FOnGetCurrentItemIndex OnGetCurrentItemIndexIn)
		: OnGetCurrentItemIndex(OnGetCurrentItemIndexIn)
	{}

	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization> MakeInstance(FOnGetCurrentItemIndex OnGetCurrentItemIndexIn);

	/** IDetailCustomization interface */
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;

	TArray<TWeakObjectPtr<UObject>> CustomizedObjects;

	FOnGetCurrentItemIndex OnGetCurrentItemIndex;
};


