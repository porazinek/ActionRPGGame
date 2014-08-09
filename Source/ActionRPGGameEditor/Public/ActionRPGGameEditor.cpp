// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGameEditor.h"
#include "PropertyEditorModule.h"

#include "Developer/AssetTools/Public/AssetToolsModule.h"

class FActionRPGGameEditor : public IModuleInterface
{

	TArray< TSharedPtr<IAssetTypeActions> > CreatedAssetTypeActions;

	virtual void StartupModule() override
	{
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
		RegisterAssetTypeAction(AssetTools, MakeShareable(new FARItemAssetTypeActions));

		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

		//PropertyModule.RegisterCustomClassLayout("ItemDataAsset", FOnGetDetailCustomizationInstance::CreateStatic(&FARItemAssetDetails::MakeInstance));
		PropertyModule.RegisterCustomPropertyTypeLayout("ARItemPickerContainer", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FARItemPickerContainerCustomization::MakeInstance));
		//PropertyModule.RegisterCustomPropertyTypeLayout("ItemDataAssetKeySelector", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FBlackboardSelectorDetails::MakeInstance));
		//PropertyModule.RegisterCustomClassLayout("ItemDataAsset", FOnGetDetailCustomizationInstance::CreateStatic(&FBlackboardDecoratorDetails::MakeInstance));
	}

	virtual void ShutdownModule() override
	{
		if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
		{
			IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
			for (int32 Index = 0; Index < CreatedAssetTypeActions.Num(); ++Index)
			{
				AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeActions[Index].ToSharedRef());
			}
		}
		CreatedAssetTypeActions.Empty();
	}

	void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
	{
		AssetTools.RegisterAssetTypeActions(Action);
		CreatedAssetTypeActions.Add(Action);
	}
};

IMPLEMENT_MODULE(FActionRPGGameEditor, ActionRPGGameEditor);