// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "AREnumTypes.h"
#include "Slate.h"
#include "ARStructTypes.generated.h"

//it probably will be better to store these informations in DataTable
//or in database. For much faster lookups.
//and the best option possible would be to create hard references at global level
//to each table, so we won't need to initialize the every time we need them...
const FString ChestDataAssetPath = "/Game/Blueprints/Items_DataAsset.Items_DataAsset";
const FString WeaponDataAssetPath = "/Game/Blueprints/Data/WeaponData.WeaponData";

const FString HeadItemDataAssetPath = "/Game/Blueprints/Data/HeadData.HeadData";
const FString ShoulderItemDataAssetPath = "/Game/Blueprints/Data/ShoulderData.ShoulderData";
const FString ChestItemDataAssetPath = "/Game/Blueprints/Data/ChestData.ChestData";
const FString HandsItemDataAssetPath = "/Game/Blueprints/Data/HandsData.HandsData";
const FString LegItemDataAssetPath = "/Game/Blueprints/Data/LegsData.LegsData";
const FString FootItemDataAssetPath = "/Game/Blueprints/Data/FootData.FootData";
const FString WeaponItemDataAssetPath = "/Game/Blueprints/Data/WeaponData.WeaponData";
static UDataTable* ChestItemDataTable;

USTRUCT(BlueprintType)
struct FAttribute
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		FName AttributeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		float ModValue;

	FAttribute() {};
};

USTRUCT(BlueprintType)
struct FItemAttribute : public FAttribute
{
	GENERATED_USTRUCT_BODY()

	//special indicator that tells if multiple attributes of the same
	//name/type should stack
	//if false only the highest attribute from all equiped items will be used.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		bool IsStackable;

	inline bool operator!= (const FItemAttribute& Other) const
	{
		return AttributeName != Other.AttributeName
			&& ModValue != Other.ModValue
			&& IsStackable != Other.IsStackable;
	};

	FItemAttribute& operator=(const FItemAttribute& Other)
	{
		if (*this != Other)
		{
			AttributeName = Other.AttributeName;
			ModValue = Other.ModValue;
			IsStackable = Other.IsStackable;
		}
		return *this;
	};
};

USTRUCT(BlueprintType)
struct FAttributeModified
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadOnly, Category="Attribute")
		FName AttributeName;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float ActuallDamage;

	FAttributeModified() {};
};

USTRUCT(BlueprintType)
struct FAttributeDamage
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		FName AttributeName;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float ActuallDamage;

	//actor which instigated damage. For example weapon or ability.
	UPROPERTY()
	class AActor* Instigated;

	//Type of Damage

	//this might be in sub struct, to accommodate for the fact there there can be more than one target
	//ie. AoE damage.
	//Who was hit
	UPROPERTY()
	AActor* Target;
	//By who (another player)
	UPROPERTY()
	AActor* CausedBy;


	FAttributeDamage() {};
};

/*
	It's struct containing info about item.
	It have advantage of being simple to store in single DataAsset, but Items can't
	be directly placed on level.

	On other hand managing all the possible actors is not better..
*/
USTRUCT(BlueprintType)
struct FARItemInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
		int32 ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
		FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
		FName AttachSocket;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
		UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
		TEnumAsByte<EItemSlot> ItemSlot;

	UPROPERTY(EditAnywhere, Category = "Item")
		TArray<FItemAttribute> Attributes;

	UPROPERTY(EditAnywhere, Category = "Item")
		TAssetPtr<USkeletalMesh> ItemMesh;

	UPROPERTY(EditAnywhere, Category = "Item")
		TSubclassOf<class AARItem> ItemType;

	UPROPERTY()
		TWeakObjectPtr<class AARItem> Item;

	UPROPERTY(EditAnywhere, Category = "Item")
		TAssetSubclassOf<class AARItem> ItemClass;

	UPROPERTY(EditAnywhere, Category = "Item")
	struct FSlateBrush SlateIcon;

	inline bool operator!= (const FARItemInfo& Other) const
	{
		return ItemName != Other.ItemName;
	};

	FARItemInfo& operator=(const FARItemInfo& Other)
	{
		if (*this != Other)
		{
			ItemID = Other.ItemID;
			ItemName = Other.ItemName;
			ItemIcon = Other.ItemIcon;
			ItemSlot = Other.ItemSlot;
			Attributes = Other.Attributes;
			ItemMesh = Other.ItemMesh;
			ItemType = Other.ItemType;
			Item = Other.Item;
		}
		return *this;
	};

	bool operator==(const FARItemInfo& Other) const
	{
		//return ItemID == Other.ItemID;
		return ItemName == Other.ItemName;
	}

	//bool operator==(const FName& Other) const
	//{
	//	//return ItemID == Other.ItemID;
	//	return ItemName == Other;
	//}

	FARItemInfo()
		: ItemID(0),
		ItemIcon(NULL),
		ItemName("Default")
	{}
};

DECLARE_DELEGATE_RetVal(TArray<FARItemInfo>, FGetItemInfo)

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
		int8 SlotID;

	UPROPERTY(EditAnywhere, Category="Item")
		FName ItemID;

	UPROPERTY(EditAnywhere, Category = "Item")
		TEnumAsByte<EItemSlot> ItemSlot; //to check which datasset we should query.

	UPROPERTY(EditAnywhere, Category = "Item")
		TEnumAsByte<EEquipmentSlot::Type> EEquipmentSlot;

	inline bool operator!= (const FInventorySlot& Other) const
	{
		return ItemID != Other.ItemID;
	};

	inline FInventorySlot& operator=(const FInventorySlot& Other)
	{
		if (*this != Other)
		{
			SlotID = Other.SlotID;
			ItemID = Other.ItemID;
			ItemSlot = Other.ItemSlot;
			EEquipmentSlot = Other.EEquipmentSlot;
		}
		return *this;
	};

	inline bool operator==(const FInventorySlot& Other) const
	{
		return ItemID == Other.ItemID;
	};
};

USTRUCT(BlueprintType)
struct FPickupItem
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Item")
		TWeakObjectPtr<class UARItemDataAsset> DataAsset;

	UPROPERTY(EditAnywhere, Category = "Item")
		FName ItemName;


	bool operator==(const FPickupItem& Other) const
	{
		return ItemName == Other.ItemName;
	}
};

USTRUCT(BlueprintType)
struct FPeriodicEffect
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Effect")
		float CurrentDuration;

	UPROPERTY(NotReplicated, BlueprintReadOnly, Category = "Effect")
		float ClientCurrentDuration;

	UPROPERTY()
		float MaxDuration;

	/*
		This actually doesn't set damage to effect.
		It's used to replicate but to client to have something to show on UI. IF needed.
		Actuall damage is set directly in blueprint and is directly attached to used effects.
	*/
	UPROPERTY()
		float FinalDamage;

	UPROPERTY()
		FName EffectName;

	UPROPERTY()
		bool IsEffectActive;

	void Tick(float DeltaTime);

	/* We don't need to replicate effect object back to user
	   That is why struct contains most of data	*/
	UPROPERTY(NotReplicated, Transient)
		TWeakObjectPtr<class AAREffectPeriodic> PeriodicEffect;

	bool operator==(const FPeriodicEffect& Other) const;

	FPeriodicEffect()
		: CurrentDuration(0.0f),
		MaxDuration(0.0f),
		IsEffectActive(false),
		EffectName("Invalid")
	{ }
};

USTRUCT(BlueprintType)
struct FAbilityInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
		int8 SlotID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		FName AbilityName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		TSubclassOf<class AARAbility> AbilityType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		TWeakObjectPtr<class AARAbility> Ability;
};

USTRUCT(BlueprintType)
struct FActivePeriodicEffects
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadOnly, Category = "Effect")
		TArray<FPeriodicEffect> ActiveEffects;
};


USTRUCT(BlueprintType)
struct FARItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	FARItemData()
		: ItemBlueprint(NULL)
	{}

	UPROPERTY(BlueprintReadOnly, Category = "Item")
		TAssetSubclassOf<class AARItem> ItemBlueprint;

};