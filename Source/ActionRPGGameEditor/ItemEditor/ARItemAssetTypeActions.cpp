// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGameEditor.h"

#include "ARItemEditor.h"

#include "ARItemAssetTypeActions.h"

FText FARItemAssetTypeActions::GetName() const
{
	return FText::FromString("Items Data");
}
FColor FARItemAssetTypeActions::GetTypeColor() const
{
	return FColor(255, 0, 0);
}
UClass* FARItemAssetTypeActions::GetSupportedClass() const
{
	return UARItemsData::StaticClass();
}
void FARItemAssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ?
		EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		if (UARItemsData* ItemData = Cast<UARItemsData>(*ObjIt))
		{
			TSharedRef<FARItemEditor> NewItemEditor(new FARItemEditor());
			NewItemEditor->InitiItemEditor(Mode, EditWithinLevelEditor, ItemData);
		}
	}
}
uint32 FARItemAssetTypeActions::GetCategories()
{
	return EAssetTypeCategories::Gameplay;
}
//virtual FText GetAssetDescription(const FAssetData& AssetData) const override;