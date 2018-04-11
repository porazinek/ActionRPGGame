// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IFInventoryComponent.generated.h"

//local index
DECLARE_MULTICAST_DELEGATE_OneParam(FIFOnItemChanged, uint8);
DECLARE_MULTICAST_DELEGATE(FIFOnInventoryChanged);

USTRUCT()
struct FIFItemData : public FFastArraySerializerItem
{
	GENERATED_BODY()
		friend class UIFInventoryComponent;
	friend struct FIFItemContainer;

protected:
	UPROPERTY()
		class UIFItemBase* Item;

	/*
		Indexes below are used to resolve item mapping between server and client,
		because there is no guarnteed order of replication.
	*/
	/*
		Index in Array of this item on Server.
	*/
	UPROPERTY()
		uint8 NetIndex;

	/*
		LIndex in Array of this item on Client.
	*/
	UPROPERTY(NotReplicated)
		uint8 LocalIndex;
public:

	FIFItemData()
		: Item(nullptr)
		, NetIndex(INDEX_NONE)
		, LocalIndex(INDEX_NONE)
	{}

	FIFItemData(uint8 InNetIndex, uint8 InLocalIndex)
		: Item(nullptr)
		, NetIndex(InNetIndex)
		, LocalIndex(InLocalIndex)
	{}

	/**
	* Called right before deleting element during replication.
	*
	* @param InArraySerializer	Array serializer that owns the item and has triggered the replication call
	*
	* NOTE: intentionally not virtual; invoked via templated code, @see FExampleItemEntry
	*/
	void PreReplicatedRemove(const struct FIFItemContainer& InArraySerializer);
	/**
	* Called after adding and serializing a new element
	*
	* @param InArraySerializer	Array serializer that owns the item and has triggered the replication call
	*
	* NOTE: intentionally not virtual; invoked via templated code, @see FExampleItemEntry
	*/
	void PostReplicatedAdd(const struct FIFItemContainer& InArraySerializer);
	/**
	* Called after updating an existing element with new data
	*
	* @param InArraySerializer	Array serializer that owns the item and has triggered the replication call
	* NOTE: intentionally not virtual; invoked via templated code, @see FExampleItemEntry
	*/
	void PostReplicatedChange(const struct FIFItemContainer& InArraySerializer);

	bool operator==(const FIFItemData& Right) const
	{
		return NetIndex == Right.NetIndex;
	}
};

USTRUCT()
struct FIFItemContainer : public FFastArraySerializer
{	
	GENERATED_BODY()
		friend class UIFInventoryComponent;
	UPROPERTY()
		TArray<FIFItemData> Items;

	TWeakObjectPtr<class UIFInventoryComponent> IC;
	/* NetIndex, LocalIndex */
	TMap<uint8, uint8> NetToLocal;
	TMap<uint8, uint8> LocalToNet;

protected:
	void AddData(const FIFItemData& InItem)
	{
		
		int32 Idx = Items.Add(InItem);
		MarkItemDirty(Items[Idx]);
		MarkArrayDirty();
		NetToLocal.Add(InItem.NetIndex, InItem.LocalIndex);
		LocalToNet.Add(InItem.LocalIndex, InItem.NetIndex);
	}
	uint8 GetNetIndex(uint8 LocalIndex)
	{
		return LocalToNet.FindRef(LocalIndex);
	}
	void AddItem(uint8 InNetIndex);
	void AddItem(class UIFItemBase* InItem, uint8 InNetIndex);
	void AddItemToFreeSlot(class UIFItemBase* InItem);
public:
	bool NetDeltaSerialize(FNetDeltaSerializeInfo & DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FIFItemData, FIFItemContainer>(Items, DeltaParms, *this);
	}
};

template<>
struct TStructOpsTypeTraits< FIFItemContainer > : public TStructOpsTypeTraitsBase2<FIFItemContainer>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYFRAMEWORK_API UIFInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated)
		FIFItemContainer Inventory;

	FIFOnItemChanged OnItemChanged;
	FIFOnInventoryChanged OnInventoryChanged;

public:	
	// Sets default values for this component's properties
	UIFInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* 
		Adds new item at slot specified slot 
		Source - Droped item from which we will transfer item
		SourceIndex - Index
	*/
	void AddItemFromActor(class AIFItemActorBase* Source
		, uint8 SourceIndex
		, uint8 InLocalIndex);
	
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAddItemFromActor(class AIFItemActorBase* Source
			, uint8 SourceIndex
			, uint8 InLocalIndex);
	void ServerAddItemFromActor_Implementation(class AIFItemActorBase* Source
		, uint8 SourceIndex
		, uint8 InNetIndex);
	bool ServerAddItemFromActor_Validate(class AIFItemActorBase* Source
		, uint8 SourceIndex
		, uint8 InNetIndex);
	UFUNCTION(BlueprintCallable, Category = "InventoryFramework")
		void BP_AddAllItemsFromActor(class AIFItemActorBase* Source);

	void AddItemFromOtherInventory(class UIFInventoryComponent* Source
		, uint8 SourceNetIndex
		, uint8 InLocalIndex);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAddItemFromOtherInventory(class UIFInventoryComponent* Source
			, uint8 SourceNetIndex
			, uint8 InLocalIndex);
	void ServerAddItemFromOtherInventory_Implementation(class UIFInventoryComponent* Source
		, uint8 SourceNetIndex
		, uint8 InLocalIndex);
	bool ServerAddItemFromOtherInventory_Validate(class UIFInventoryComponent* Source
		, uint8 SourceNetIndex
		, uint8 InLocalIndex);

	/* Adds item from class. Realistically you should never call it on client. */
	void AddItemFromClass(TSoftClassPtr<class UIFItemBase> Item, uint8 InLocalIndex);
	
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAddItemFromClass(const FSoftObjectPath& Item, uint8 InLocalIndex);
	
	void ServerAddItemFromClass_Implementation(FSoftObjectPath Item, uint8 InNetIndex);
	bool ServerAddItemFromClass_Validate(FSoftObjectPath Item, uint8 InNetIndex);
	UFUNCTION(BlueprintCallable, Category = "InventoryFramework")
		void BP_AddItemFromClass(TSoftClassPtr<class UIFItemBase> Item, uint8 InLocalIndex);

	UFUNCTION()
		void OnItemLoadedFreeSlot(TSoftClassPtr<class UIFItemBase> InItem);

	UFUNCTION()
		void OnItemLoaded(TSoftClassPtr<class UIFItemBase> InItem, uint8 InNetIndex);

	bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags) override;
};