// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGameEditor.h"

#include "ARItemEditor.h"

#include "ARItemAssetDetails.h"

TSharedRef<IDetailCustomization> FARItemAssetDetails::MakeInstance(FOnGetCurrentItemIndex OnGetCurrentItemIndexIn)
{
	return MakeShareable(new FARItemAssetDetails(OnGetCurrentItemIndexIn));
}

void FARItemAssetDetails::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{
	// First hide all keys
	DetailLayout.HideProperty(TEXT("TestNAmes"));
	//DetailLayout.GetObjectsBeingCustomized(CustomizedObjects);
	DetailLayout.HideProperty(TEXT("EditEntries"));
	DetailLayout.HideProperty(TEXT("CurrentEntry"));


	//// Now show only the currently selected key
	int32 CurrentSelection = INDEX_NONE;
	if (OnGetCurrentItemIndex.IsBound())
	{
		CurrentSelection = OnGetCurrentItemIndex.Execute();
	}

	if (CurrentSelection >= 0)
	{
		TSharedPtr<IPropertyHandle> KeysHandle =  DetailLayout.GetProperty(TEXT("EditEntries"));
		check(KeysHandle.IsValid());
		uint32 NumChildKeys = 0;
		KeysHandle->GetNumChildren(NumChildKeys);
		if ((uint32)CurrentSelection < NumChildKeys)
		{
			TSharedPtr<IPropertyHandle> KeyHandle = KeysHandle->GetChildHandle((uint32)CurrentSelection);
			TSharedPtr<IPropertyHandle> Data = KeyHandle->GetChildHandle(TEXT("ItemDataInfo"));
			IDetailCategoryBuilder& DetailCategoryBuilder = DetailLayout.EditCategory("ItemData");
			
//			Data->

			DetailCategoryBuilder.AddProperty(Data);
			
		}
	}
}
