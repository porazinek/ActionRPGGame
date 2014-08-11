// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGameEditor.h"
#include "Toolkits/IToolkitHost.h"
#include "Editor/WorkspaceMenuStructure/Public/WorkspaceMenuStructureModule.h"

#include "Editor/PropertyEditor/Private/PropertyChangeListener.h"
#include "Editor/PropertyEditor/Public/IPropertyChangeListener.h"

#include "ARItemEditor.h"

const FName FARItemEditor::ItemEditorTabId(TEXT("ItemEditor_Item"));
const FName FARItemEditor::ItemEditorTabDetails(TEXT("ItemEditor_Details"));

void SAddItemOptions::Construct(const FArguments& InArgs)
{
	ItemAsset = InArgs._ItemAsset;
	OwningWindow = InArgs._OwningWindow;
	ItemList = InArgs._ItemList;
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.MaxHeight(22)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.MaxWidth(60)
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Right)
			.Padding(4,4)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Item Name: "))
			]
			+ SHorizontalBox::Slot()
			.MaxWidth(140)
			.Padding(4,4)
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			[
				SNew(SEditableTextBox)
				.MinDesiredWidth(140)
				.OnTextCommitted(this, &SAddItemOptions::SetItemName)
			]
		]
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		.MaxHeight(60)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.MaxWidth(80)
			.Padding(4, 4)
			[
				SNew(SButton)
				.OnClicked(this, &SAddItemOptions::OnAddItem)
				[
					SNew(STextBlock)
					.Text(FText::FromString("Add Item"))
				]
			]
			+ SHorizontalBox::Slot()
			.MaxWidth(80)
			.Padding(4,4)
			[
				SNew(SButton)
				.OnClicked(this, &SAddItemOptions::OnCancel)
				[
					SNew(STextBlock)
					.Text(FText::FromString("Cancel"))
				]
			]
		]
	];
}

void SAddItemOptions::SetItemName(const FText& TextIn, ETextCommit::Type CommitType)
{
	ItemName = TextIn.ToString();
}

FReply SAddItemOptions::OnAddItem()
{
	if (ItemAsset.IsValid())
	{
		FARItemInfo data;
		data.ItemName = ItemName;

		ItemAsset->AddItem(data);

		ItemList.Pin()->GraphActionMenu->RefreshAllActions(true, true);

		if (OwningWindow.IsValid())
		{
			OwningWindow.Pin()->RequestDestroyWindow();
		}
	}
	return FReply::Handled();
}

FReply SAddItemOptions::OnCancel()
{
	if (OwningWindow.IsValid())
	{
		OwningWindow.Pin()->RequestDestroyWindow();
	}

	return FReply::Handled();
}

class FEdGraphSchemaAction_RPGItemEntry : public FEdGraphSchemaAction_Dummy
{
public:
	static FString StaticGetTypeId() { static FString Type = TEXT("FEdGraphSchemaAction_RPGItemEntry "); return Type; }
	virtual FString GetTypeId() const override { return StaticGetTypeId(); }

	FEdGraphSchemaAction_RPGItemEntry(UARItemsData* ItemsDataIn, FName InKey, FString ItemNameIn, int32 ItemIndexIn)
		: FEdGraphSchemaAction_Dummy()
		, ItemsData(ItemsDataIn)
		, ItemName(ItemNameIn)
		, Key(InKey)
		, ItemIndex(ItemIndexIn)
	{
		check(ItemsData);
		Update();
	}

	void Update()
	{
		MenuDescription = FText::FromString(ItemName);
		TooltipDescription = FText::FromName(Key).ToString(); // LOCTEXT("BlackboardEntryFormat", "{0}"), FText::FromName(Key).ToString());
	}

	UARItemsData* ItemsData;
	FString ItemName;
	FName Key;
	int32 ItemIndex;
};

class SRPGItem : public SGraphPaletteItem
{
	SLATE_BEGIN_ARGS(SRPGItem) {}


	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, FCreateWidgetForActionData* const InCreateData)
	{

		const FSlateFontInfo NameFont = FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 10);

		check(InCreateData);
		check(InCreateData->Action.IsValid());

		TSharedPtr<FEdGraphSchemaAction> GraphAction = InCreateData->Action;
		check(GraphAction->GetTypeId() == FEdGraphSchemaAction_RPGItemEntry::StaticGetTypeId());
		TSharedPtr<FEdGraphSchemaAction_RPGItemEntry> BlackboardEntryAction = StaticCastSharedPtr<FEdGraphSchemaAction_RPGItemEntry>(GraphAction);

		ActionPtr = InCreateData->Action;

		FSlateBrush const* IconBrush = FEditorStyle::GetBrush(TEXT("NoBrush"));

		TSharedRef<SWidget> IconWidget = CreateIconWidget(GraphAction->TooltipDescription, IconBrush, IconBrush->TintColor);
		//TSharedRef<SWidget> NameSlotWidget = CreateTextSlotWidget(NameFont, InCreateData, false);

		// Create the actual widget
		this->ChildSlot
			[
				SNew(SHorizontalBox)
				// Name slot
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(3, 0)
				[
					SNew(STextBlock)
					.Text(this, &SRPGItem::GetDisplayText)
				]
			];
	}
};


void SItemListWidget::Construct(const FArguments& InArgs)
{
	ItemAsset = InArgs._ItemAsset;
	OnItemSelected = InArgs._OnItemSelected;
	SyncItemList();

	ChildSlot
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				SAssignNew(GraphActionMenu, SGraphActionMenu, false)
				.OnCreateWidgetForAction(this, &SItemListWidget::HandleCreateWidgetForAction)
				.OnCollectAllActions(this, &SItemListWidget::HandleCollectAllActions)
				.OnActionSelected(this, &SItemListWidget::HandleActionSelected)

			]
		];
}
TSharedRef<SWidget> SItemListWidget::HandleCreateWidgetForAction(FCreateWidgetForActionData* const InCreateData)
{
	return SNew(SRPGItem, InCreateData);
}
void SItemListWidget::HandleCollectAllActions(FGraphActionListBuilderBase& GraphActionListBuilder)
{
	if (ItemAsset != nullptr)
	{
		for (FItemEntry& Key : ItemAsset->EditEntries)
		{
			GraphActionListBuilder.AddAction(MakeShareable(new FEdGraphSchemaAction_RPGItemEntry(ItemAsset, Key.Key, Key.ItemDataInfo.ItemName, Key.Index)));
		}
	}
}
void SItemListWidget::HandleActionSelected(const TArray< TSharedPtr<FEdGraphSchemaAction> >& SelectedActions) const
{
	if (SelectedActions.Num() > 0)
	{
		check(SelectedActions[0]->GetTypeId() == FEdGraphSchemaAction_RPGItemEntry::StaticGetTypeId());
		TSharedPtr<FEdGraphSchemaAction_RPGItemEntry> ItemEntry = StaticCastSharedPtr<FEdGraphSchemaAction_RPGItemEntry>(SelectedActions[0]);
		OnItemSelected.ExecuteIfBound(ItemEntry->Key, ItemEntry->ItemIndex);
	}
}

TSharedPtr<FEdGraphSchemaAction_RPGItemEntry> SItemListWidget::GetSelectedEntryInternal() const
{
	TArray< TSharedPtr<FEdGraphSchemaAction> > SelectedActions;
	GraphActionMenu->GetSelectedActions(SelectedActions);

	if (SelectedActions.Num() > 0)
	{
		check(SelectedActions[0]->GetTypeId() == FEdGraphSchemaAction_RPGItemEntry::StaticGetTypeId());
		return StaticCastSharedPtr<FEdGraphSchemaAction_RPGItemEntry>(SelectedActions[0]);
	}

	return TSharedPtr<FEdGraphSchemaAction_RPGItemEntry>();
}

void SItemListWidget::SyncItemList()
{
	if (ItemAsset)
	{
		for (auto It = ItemAsset->ItemMap.CreateIterator(); It; ++It)
		{
			ItemList.Add(MakeShareable(new FName(It.Key())));
		}
	}
}
FReply SItemListWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Unhandled();
}

int32 SItemListWidget::GetItemIndex()
{
	TSharedPtr<FEdGraphSchemaAction_RPGItemEntry> Entry = GetSelectedEntryInternal();
	if (Entry.IsValid())
	{
		//Entry->Key;
		return Entry->ItemIndex;
	}
	return CurrentIndex;
}
TSharedRef<ITableRow> SItemListWidget::MakeTileViewWidget(TSharedPtr<FName> AssetItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	//if (AssetItem.IsValid())
	//{
	//	FName nam = *AssetItem.Get();
	//	TSharedRef< STableRow<TSharedPtr<FName> >> ReturnRow = SNew(STableRow<TSharedPtr<FName>>, OwnerTable)
	//		.Content()
	//		[
	//			SNew(SItemSingleWidget)
	//			.OnItemSelected(OnItemSelected)
	//			.Key(nam)
	//			.ItemAsset(ItemAsset)
	//			.ItemEntry(this)
	//		];
	//	return ReturnRow;
	//}
	TSharedPtr< STableRow<TSharedPtr<FName>> > TableRowWidget;
	return TableRowWidget.ToSharedRef();
}

void FARItemEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(TabManager);
	const IWorkspaceMenuStructure& MenuStructure = WorkspaceMenu::GetMenuStructure();

	TabManager->RegisterTabSpawner(ItemEditorTabId, FOnSpawnTab::CreateSP(this, &FARItemEditor::SpawnTab_Item))
		.SetDisplayName(FText::FromString("Item Editor"))
		.SetGroup(MenuStructure.GetAssetEditorCategory());
	TabManager->RegisterTabSpawner(ItemEditorTabDetails, FOnSpawnTab::CreateSP(this, &FARItemEditor::SpawnTab_Details))
		.SetDisplayName(FText::FromString("Item Details"))
		.SetGroup(MenuStructure.GetAssetEditorCategory());
}
void FARItemEditor::UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(TabManager);
	TabManager->UnregisterTabSpawner(ItemEditorTabId);
	TabManager->UnregisterTabSpawner(ItemEditorTabDetails);
}

FARItemEditor::~FARItemEditor()
{
	ItemDetailsView.Reset();
	ItemEntry.Reset();
}

void FARItemEditor::InitiItemEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UARItemsData* ItemIn)
{
	EditedItemAsset = ItemIn;

	const bool bIsUpdatable = false;
	const bool bAllowFavorites = true;
	const bool bIsLockable = false;
	const bool bAllowSearch = true;
	const bool bObjectsUseNameArea = false;
	const bool bHideSelectionTip = false;
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs(bIsUpdatable, bIsLockable, bAllowSearch, bObjectsUseNameArea, bHideSelectionTip);

	ItemDetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);

	FOnGetCurrentItemIndex OnGetCurrentItemIndex = FOnGetCurrentItemIndex::CreateSP(this, &FARItemEditor::GetCurrentIndex);
	FOnGetDetailCustomizationInstance LayoutVariableDetails = FOnGetDetailCustomizationInstance::CreateStatic(&FARItemAssetDetails::MakeInstance, OnGetCurrentItemIndex);
	ItemDetailsView->RegisterInstancedCustomPropertyLayout(UARItemsData::StaticClass(), LayoutVariableDetails);

	ItemDetailsView->SetObject(ItemIn);

	ItemDetailsView->OnFinishedChangingProperties().AddRaw(this, &FARItemEditor::SaveEditedPRoperties);
	

	const TSharedRef<FTabManager::FLayout> RPGItemEditorDefault = FTabManager::NewLayout("Standalone_ItemEditor")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.2f)
				->SetHideTabWell(true)
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
			)
			->Split
			(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Horizontal)
				->SetSizeCoefficient(0.8f)
				->Split
				(
					FTabManager::NewSplitter()
					->SetOrientation(Orient_Vertical)
					->SetSizeCoefficient(0.2f)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.75f)
						->AddTab(ItemEditorTabId, ETabState::OpenedTab)
					)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.25f)
						->AddTab(ItemEditorTabDetails, ETabState::OpenedTab)
					)
				)
			)
		);

	

	ItemEntry = SNew(SItemListWidget)
		.OnItemSelected(this, &FARItemEditor::HandleBlackboardEntrySelected)
		.ItemAsset(EditedItemAsset);
	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	InitAssetEditor(Mode, InitToolkitHost, FName("RPGItemEditor")
		, RPGItemEditorDefault, true, true,
		ItemIn);

	RegenerateMenusAndToolbars();
}

void FARItemEditor::HandleBlackboardEntrySelected(FName BlackboardEntry, int32 ItemIndex)
{
	//int32 Idx = 0;
	//for (FItemEntry& item : EditedItemAsset->EditEntries)
	//{
	//	if (item.Key == BlackboardEntry)
	//	{
	//		CurrentIndex = Idx;
	//		break;
	//	}
	//	Idx++;
	//}
	// refresh details view
	const bool bForceRefresh = true;
	ItemDetailsView->SetObject(EditedItemAsset, bForceRefresh);
	CurrentIndex = ItemIndex;
	//EditedItemAsset->SetItemInMap(CurrentIndex);
}
// FAssetEditorToolkit
FName FARItemEditor::GetToolkitFName() const
{
	return FName("RPGItemEditor");
}
FText FARItemEditor::GetBaseToolkitName() const
{
	return FText::FromString("Item Editor");
}
FText FARItemEditor::GetToolkitName() const
{
	return FText::FromString("Item Editor");
}
FLinearColor FARItemEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor::Red;
}
FString FARItemEditor::GetWorldCentricTabPrefix() const
{
	return TEXT("ItemEditor");
}

FString FARItemEditor::GetDocumentationLink() const
{
	return FString("No Link!");
}
// End of FAssetEditorToolkit

TSharedRef<SWidget> FARItemEditor::SetItemDetails()
{

	const bool bIsUpdatable = false;
	const bool bAllowFavorites = true;
	const bool bIsLockable = false;
	const bool bAllowSearch = true;
	const bool bObjectsUseNameArea = false;
	const bool bHideSelectionTip = true;
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs(bIsUpdatable, bIsLockable, bAllowSearch, bObjectsUseNameArea, bHideSelectionTip);

	ItemDetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);

	FOnGetCurrentItemIndex OnGetCurrentItemIndex = FOnGetCurrentItemIndex::CreateSP(this, &FARItemEditor::GetCurrentIndex);
	FOnGetDetailCustomizationInstance LayoutVariableDetails = FOnGetDetailCustomizationInstance::CreateStatic(&FARItemAssetDetails::MakeInstance, OnGetCurrentItemIndex);
	ItemDetailsView->RegisterInstancedCustomPropertyLayout(UARItemsData::StaticClass(), LayoutVariableDetails);

	ItemDetailsView->SetObject(EditedItemAsset);


	return ItemDetailsView.ToSharedRef();
}

int32 FARItemEditor::GetCurrentIndex()
{
	if (ItemEntry.IsValid())
		return ItemEntry->GetItemIndex();
	return INDEX_NONE;
	//return CurrentIndex;
}

void FARItemEditor::SaveEditedPRoperties(const FPropertyChangedEvent& EventIn)
{
	//UArrayProperty* tempArray
	int32 indx = CurrentIndex;
	EditedItemAsset->SetItemInMap(CurrentIndex);
	//ItemEntry->GraphActionMenu->RefreshAllActions(true, true);
}

TSharedRef<SDockTab> FARItemEditor::SpawnTab_Item(const FSpawnTabArgs& Args)
{
	//check(Args.GetTabId().TabType == ItemEditorTabId);

	return SNew(SDockTab)
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			[
				ItemEntry.ToSharedRef()
			]
			+ SVerticalBox::Slot()
			[
				SNew(SButton)
				.OnClicked(this, &FARItemEditor::CreateNewItem)
				[
					SNew(STextBlock)
					.Text(FText::FromString("Add New Item"))
				]
			]
		]
	];
}

TSharedRef<SDockTab> FARItemEditor::SpawnTab_Details(const FSpawnTabArgs& Args)
{
	//check(Args.GetTabId().TabType == ItemEditorTabDetails);

	return SNew(SDockTab)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			[
				ItemDetailsView.ToSharedRef()
			]
		];
}

FReply FARItemEditor::CreateNewItem()
{
	TSharedPtr<SWindow> ParentWindow;
	TSharedPtr<SAddItemOptions> WindowContent;
	if (FModuleManager::Get().IsModuleLoaded("MainFrame"))
	{
		IMainFrameModule& MainFrame = FModuleManager::Get().LoadModuleChecked<IMainFrameModule>("MainFrame");
		ParentWindow = MainFrame.GetParentWindow();
	}
	TSharedRef<SWindow> Window = SNew(SWindow)
		.Title(FText::FromString("New Item"))
		.SizingRule(ESizingRule::Autosized);

	Window->SetContent
		(
			SAssignNew(WindowContent, SAddItemOptions).ItemAsset(EditedItemAsset)
			.OwningWindow(Window)
			.ItemList(ItemEntry)
		);
	FSlateApplication::Get().AddModalWindow(Window, ParentWindow, false);

	//if (EditedItemAsset)
	//{
	//	EditedItemAsset->AddNewItem();
	//ItemEntry->GraphActionMenu->RefreshAllActions(true, true);
	//}
	return FReply::Handled();
}
