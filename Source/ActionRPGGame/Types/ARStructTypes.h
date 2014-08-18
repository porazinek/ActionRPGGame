// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "AREnumTypes.h"
#include "Slate.h"
#include "Slate/SlateBrushAsset.h"

#include "ARAttributeTypes.h"
#include "ARStructTypes.generated.h"

DECLARE_DELEGATE_RetVal(float, FOnGetFloat);



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

//this is probably not the best idea around.
static UDataTable* ChestItemDataTable;
static UDataTable* WeaponItemDataTable;


USTRUCT(BlueprintType)
struct FAttributeDamage
{
	GENERATED_USTRUCT_BODY()
public:
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


USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
		int8 SlotID;
	UPROPERTY()
		int8 OldSlotID;

	UPROPERTY(EditAnywhere, Category="Item")
		FName ItemID;

	UPROPERTY(EditAnywhere, Category = "Item")
		int32 ItemIndex;

	UPROPERTY(EditAnywhere, Category = "Item")
		bool IsAttached;

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
			OldSlotID = Other.OldSlotID;
			ItemIndex = Other.ItemIndex;
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
struct FARAbilityData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, Category="Index")
		int32 AbilityIndex;

	UPROPERTY(EditAnywhere, Category = "Action Class")
		TSubclassOf<class AARAbility> AbilityClass;

	UPROPERTY(EditAnywhere, Category = "Icon")
		FSlateBrush AbilityIcon;
};


USTRUCT(BlueprintType)
struct FActionSlotInfo
{
	GENERATED_USTRUCT_BODY()
public:
	/* Must be the same as Index in array */
	UPROPERTY(EditAnywhere, Category="Slot Index")
		int32 SlotIndex;
	UPROPERTY()
		int32 OldSlotIndex;
	UPROPERTY()
		int32 ActionBarIndex;

	UPROPERTY()
		int32 ActionIndex;

	UPROPERTY(BlueprintReadWrite, Category = "Ability Object")
		TWeakObjectPtr<class AARAbility> Ability;

	UPROPERTY()
		float CurrentCooldown;
};

/* Essentialy represents single action bar */
USTRUCT(BlueprintType)
struct FActionSlotContainer
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Slot Index")
		int32 ActionBarIndex;

	UPROPERTY(EditAnywhere, Category = "Action Slots")
	TArray<FActionSlotInfo> ActionSlots;

	FActionSlotContainer()
		: ActionBarIndex(INDEX_NONE)
		, ActionSlots()
	{}
};

/* Contains  multiple action bars */
USTRUCT(BlueprintType)
struct FActionBarContainer
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Action Bars")
	TArray<FActionSlotContainer> ActionBars;

	FActionBarContainer()
		: ActionBars()
	{}
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

	UPROPERTY(BlueprintReadOnly, Category = "Item")
		TAssetPtr<USlateBrushAsset> ItemIcon;

	UPROPERTY(BlueprintReadOnly, Category = "Item")
		TArray<FAttribute> Attribute;
};
/*
	Keep in mind that those properties, do not really have any real world physical
	meaning. They exist as way for easier setup for various substances.

	In reality there is many prameters that may affect how easy to trun something in flames.
	While freeze or Melting properties are pretty straight forward and should be used with values close to 
	real world materials.
*/
USTRUCT(BlueprintType)
struct FARActorSubstanceProperties
{
	GENERATED_USTRUCT_BODY()
public:
	/*
	How much fuel object have. When fuel is 0, object is destroyed.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Properties")
		float Fuel;
	/*
	Current temperature of object. In Celcious.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Properties")
		float Temperature;
	/*
	Minimum needed temperature to ignite object. In Celcious.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Properties")
		float CombustionTemperature;
	/*
	Minimum needed temperature to freeze object. In Celcious.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Properties")
		float FreezeTemperature;

};

USTRUCT(BlueprintType)
struct FARProjectileInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		FVector Location;
	/*
		Initial velocity of projectile.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float InitialVelocity;
	/*
		Maximum velocity to which projectile can accellerate
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float MaxVelocity;
	/*
		Scale of gravity which affect projectile. 0 - no gravity
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float GravityScale;
	/*
		Radius in which projectile will be spawned. Used for Area Spawn. 0 -  directly at impact point.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float MaxRadius;
	/*
		Height at which projectile will be spawned.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float MaxHeight;
	/*
		Random offset which is subtracted or added to MaxHeight. Random number is generated
		from value specified in this property.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float MaxHeightOffset;
	/*
		Direction in which projectile will be moved from MaxHeigh. 0 - straight line.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float ImpactDirection;
	/*
		How many projectiles will be spawned in single burst. 0 - no projectiles will be spawned.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float BurstSize;
	/*
		Maximum Duration over which projectiles will be spawned.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float MaximumDuration;
	/*
		Time between projectile spawns.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float Interval;
	
	
};

/*
	I'm not sure if these structs are best idea, but I need some way of storing data current action
	in advance before it is used. So I can modify it in other objects.
*/
USTRUCT(BlueprintType)
struct FARActionInfo
{
	GENERATED_USTRUCT_BODY()
public:
};

/*
	I wonder if not just use AttributeComponent, or create separate Attribute Object (Subobject)
*/
USTRUCT(BlueprintType)
struct FARWeaponInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
		float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
		float MaximumSpread;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
		float RateOfFire;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
		float ReloadTime;
	/*
		Time between single shots. To prevent player for maniacally clicking fire button ;)
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
		float RefireTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
		float ClipSize;
};
USTRUCT(BlueprintType)
struct FARAbilityInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info")
		FAttribute AttriuteMod;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info")
		float RechargeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info")
		float CastTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info")
		TArray<FAttribute> ResourceCost;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info")
};

USTRUCT(BlueprintType)
struct FARUIDamage
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "UI")
		float Value;
	UPROPERTY(BlueprintReadOnly, Category = "UI")
		FVector Location;

	UPROPERTY()
		AActor* DamageInstigator;
};

USTRUCT(BlueprintType)
struct FARAttachmentSocket
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Socket")
		FName SocketName;

	UPROPERTY()
		FName LastItemID;
	/*
		What kind of item can be attached to this socket.
	*/
	UPROPERTY(EditAnywhere, Category = "Socket")
		TEnumAsByte<EAttachmentType> AttachType;

	UPROPERTY(EditAnywhere, Category = "Socket")
		TEnumAsByte<ESocketSideType> SocketSide;
	/*
		Detailed type of socket.
	*/
	UPROPERTY(EditAnywhere, Category = "Socket")
		TEnumAsByte<EAttachSocketType> SocketType;

	UPROPERTY(EditAnywhere, Category = "Socket")
		bool IsSlotAvailable;
	/*
		Montage to play when attaching/detaching from this socket.
		Maybe split it into two montages ?
	*/
	UPROPERTY(EditAnywhere, Category = "Socket")
		UAnimMontage* EquipMontage;
};



USTRUCT(BlueprintType)
struct FARDragDropInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
		int8 SlotIndex;
	UPROPERTY()
		int8 OldSlotIndex;
	UPROPERTY()
		FName SlotName;

	UPROPERTY(EditAnywhere, Category = "Item")
		FName ItemKey;

	UPROPERTY(EditAnywhere, Category = "Item")
		int32 ItemIndex;

	UPROPERTY(EditAnywhere, Category = "Item")
		bool IsAttached;

	UPROPERTY(EditAnywhere, Category = "Item")
		TEnumAsByte<EItemSlot> ItemSlot; //to check which datasset we should query.

	UPROPERTY(EditAnywhere, Category = "Item")
		TEnumAsByte<EDragDropSlot::Type> DragDropSlot;

	inline bool operator!= (const FARDragDropInfo& Other) const
	{
		return ItemKey != Other.ItemKey;
	};

	inline FARDragDropInfo& operator=(const FARDragDropInfo& Other)
	{
		if (*this != Other)
		{
			SlotIndex = Other.SlotIndex;
			OldSlotIndex = Other.OldSlotIndex;
			ItemIndex = Other.ItemIndex;
			ItemKey = Other.ItemKey;
			ItemSlot = Other.ItemSlot;
			DragDropSlot = Other.DragDropSlot;
		}
		return *this;
	};

	inline bool operator==(const FARDragDropInfo& Other) const
	{
		return ItemKey == Other.ItemKey;
	};

	FARDragDropInfo()
		: SlotIndex(INDEX_NONE)
		, OldSlotIndex(INDEX_NONE)
		, SlotName(NAME_None)
		, ItemKey(NAME_None)
		, ItemIndex(INDEX_NONE)
		, IsAttached(false)
		, ItemSlot(EItemSlot::Item_MAX)
		, DragDropSlot(EDragDropSlot::DROP__MAX)
	{}
};

//DELCARE_DELEGATE_RetVal(TArray<FInventorySlot>, FOnGetLootableItems);