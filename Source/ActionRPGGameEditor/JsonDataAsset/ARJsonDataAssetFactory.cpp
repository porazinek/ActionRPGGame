// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "ActionRPGGameEditor.h"

#include "UnrealEd.h"
#include "Factories.h"

#include "AssetRegistryModule.h"
#include "Editor/ContentBrowser/Public/ContentBrowserModule.h"
#include "Editor/ClassViewer/Public/ClassViewerModule.h"
#include "Editor/ClassViewer/Public/ClassViewerFilter.h"
#include "SClassPickerDialog.h"
#include "MessageLog.h"
#include "EnumEditorUtils.h"
#include "StructureEditorUtils.h"

#include "Serialization/Json/Json.h"

#include "Editor/MainFrame/Public/MainFrame.h"

#include "Runtime/JsonUtilities/Public/JsonUtilities.h"

#include "ARJsonDataAssetFactory.h"


class SJsonImportOption : public SCompoundWidget
{
private:
	/** Whether we should go ahead with import */
	bool bImport;

	/** Window that owns us */
	TWeakPtr< SWindow > WidgetWindow;

	// Row type

	/** Array of row struct options */
	TArray< UScriptStruct* > RowStructs;

	/** The row struct combo box */
	TSharedPtr< SComboBox<UScriptStruct*> > RowStructCombo;

	/** The selected row struct */
	UScriptStruct* SelectedStruct;

public:
	SLATE_BEGIN_ARGS(SJsonImportOption)
		: _WidgetWindow()
	{}

	SLATE_ARGUMENT(TSharedPtr<SWindow>, WidgetWindow)
		SLATE_END_ARGS()

		SJsonImportOption()
		: bImport(false)
		//, SelectedStruct(NULL)
	{}

	void Construct(const FArguments& InArgs)
	{
		WidgetWindow = InArgs._WidgetWindow;

		// Find table row struct info
		UScriptStruct* TableRowStruct = FindObjectChecked<UScriptStruct>(ANY_PACKAGE, TEXT("JsonDataObject"));
		if (TableRowStruct != NULL)
		{
			// Make combo of table rowstruct options
			for (TObjectIterator<UScriptStruct> It; It; ++It)
			{
				UScriptStruct* Struct = *It;
				// If a child of the table row struct base, but not itself
				if (Struct->IsChildOf(TableRowStruct) && Struct != TableRowStruct)
				{
					RowStructs.Add(Struct);
				}
			}
		}

		// Create widget
		this->ChildSlot
			[
				SNew(SBorder)
				//.BorderImage(FEditorStyle::GetBrush(TEXT("Menu.Background")))
				.Padding(10)
				[
					SNew(SVerticalBox)
					// Data row struct
					+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(STextBlock)
							.Text(FText::FromString("Choose JsonObject Type:"))
						]
					+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SAssignNew(RowStructCombo, SComboBox<UScriptStruct* >)
							.OptionsSource(&RowStructs)
							.OnGenerateWidget(this, &SJsonImportOption::MakeRowStructItemWidget)
							[
								SNew(STextBlock)
								.Text(this, &SJsonImportOption::GetSelectedRowOptionText)
							]
						]
					// Ok/Cancel
					+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.AutoWidth()
							[
								SNew(SButton)
								.Text(FText::FromString("OK"))
								.OnClicked(this, &SJsonImportOption::OnImport)
							]
							+ SHorizontalBox::Slot()
								.AutoWidth()
								[
									SNew(SButton)
									.Text(FText::FromString("Cancel"))
									.OnClicked(this, &SJsonImportOption::OnCancel)
								]
						]
				]
			];

	}

	/** Get the row struct we selected */
	UScriptStruct* GetSelectedRowStruct()
	{
		return SelectedStruct;
	}

	/** Called to create a widget for each struct */
	TSharedRef<SWidget> MakeRowStructItemWidget(UScriptStruct*Struct)
	{
		check(Struct);
		return	SNew(STextBlock)
			.Text(Struct->GetName());
	}


	/** Called when 'OK' button is pressed */
	FReply OnImport()
	{
		SelectedStruct = RowStructCombo->GetSelectedItem();
		bImport = true;
		if (WidgetWindow.IsValid())
		{
			WidgetWindow.Pin()->RequestDestroyWindow();
		}
		return FReply::Handled();
	}

	/** Called when 'Cancel' button is pressed */
	FReply OnCancel()
	{
		bImport = false;
		if (WidgetWindow.IsValid())
		{
			WidgetWindow.Pin()->RequestDestroyWindow();
		}
		return FReply::Handled();
	}

	FString GetSelectedRowOptionText() const
	{
		UScriptStruct* SelectedScript = RowStructCombo->GetSelectedItem();
		return (SelectedScript)
			? SelectedScript->GetName()
			: FString();
	}
};

UARJsonDataAssetFactory::UARJsonDataAssetFactory(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	bCreateNew = false;
	bEditAfterNew = true;

	bEditorImport = true;
	bText = true;

	SupportedClass = UJsonDataAsset::StaticClass();

	Formats.Add(TEXT("json;JSON Data"));
}


UObject* UARJsonDataAssetFactory::FactoryCreateText(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const TCHAR*& Buffer, const TCHAR* BufferEnd, FFeedbackContext* Warn)
{
	FEditorDelegates::OnAssetPreImport.Broadcast(this, InClass, InParent, InName, Type);

	// Save off information if so
	bool bHaveInfo = false;

	bool bDoImport = true;

	UJsonDataAsset* NewAsset = nullptr;
	TSharedPtr<SWindow> ParentWindow;
	//FJsonDataObject ImportStruct;// = nullptr;
	// Check if the main frame is loaded.  When using the old main frame it may not be.
	if (FModuleManager::Get().IsModuleLoaded("MainFrame"))
	{
		IMainFrameModule& MainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
		ParentWindow = MainFrame.GetParentWindow();
	}

	TSharedPtr<SJsonImportOption> ImportOptionsWindow;

	TSharedRef<SWindow> Window = SNew(SWindow)
		.Title(FText::FromString("DataTable Options"))
		.SizingRule(ESizingRule::Autosized);

	Window->SetContent
		(
		SAssignNew(ImportOptionsWindow, SJsonImportOption)
		.WidgetWindow(Window)
		);

	FSlateApplication::Get().AddModalWindow(Window, ParentWindow, false);

	//ImportStruct = UStruct(EStaticConstructor::EC_StaticConstructor, ImportOptionsWindow->GetSelectedRowStruct()->PropertiesSize, EObjectFlags::RF_DefaultSubObject);
	
	const FString NameForError(InName.ToString());
	const FString FileContent(BufferEnd - Buffer, Buffer);

	TSharedPtr<FJsonObject> JsonData = ParseJSON(FileContent, NameForError);

	FMyJsonContainer OutData;

	const TSharedPtr<FJsonObject> OutProperty = JsonData->GetObjectField("MyData");

	FJsonObjectConverter::JsonObjectToUStruct(OutProperty.ToSharedRef(), OutData.StaticStruct(), &OutData, 0, 0);
	
	if (bDoImport)
	{
		UJsonDataAsset* NewTable = CastChecked<UJsonDataAsset>(StaticConstructObject(UJsonDataAsset::StaticClass(), InParent, InName, Flags));
		NewTable->ImportedJsonObject = JsonData;
	//	NewTable->JsonObjectStruct = ImportStruct;
		NewTable->SetItemMap();
		NewAsset = NewTable;
	}

	FEditorDelegates::OnAssetPostImport.Broadcast(this, NewAsset);

	return NewAsset;
}


TSharedPtr<FJsonObject> UARJsonDataAssetFactory::ParseJSON(const FString& InData, const FString& ErrorNames)
{
	if (!InData.IsEmpty())
	{
		const TSharedRef<TJsonReader<>>& Reader = TJsonReaderFactory<>::Create(InData);
		TSharedPtr<FJsonObject> JsonData;
		if (FJsonSerializer::Deserialize(Reader, JsonData) && JsonData.IsValid())
		{
			return JsonData;
		}

		return JsonData;
	}
	else
	{
		return NULL;
	}
	return NULL;
}