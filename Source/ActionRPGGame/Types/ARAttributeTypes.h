// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "AREnumTypes.h"
#include "Slate.h"
#include "ARAttributeTypes.generated.h"

USTRUCT(BlueprintType)
struct FAttribute
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		FName AttributeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		float ModValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		TEnumAsByte<EAttrOp> OperationType;
	FAttribute() {};
};

USTRUCT(BlueprintType)
struct FAttributeModified
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		FName AttributeName;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float ActuallDamage;

	FAttributeModified() {};
};

/*
	It's not really damage. Attribute can have all math operations.
	Whether we can call it demegae depnds on math operation and which attribute it affect.
*/

USTRUCT(BlueprintType)
struct FAttributeChanged
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "Attribute")
		FAttribute Attribute;

	UPROPERTY(BlueprintReadWrite, Category = "Attribute")
		AActor* ChangeTarget;

	UPROPERTY(BlueprintReadWrite, Category = "Attribute")
		AActor* ChangeCauser;

	UPROPERTY(BlueprintReadWrite, Category = "Attribute")
		class AARPlayerController* ChangeInstigator;

	UPROPERTY(BlueprintReadWrite, Category = "Attribute")
		UDamageType* DamageType;
};

USTRUCT(BlueprintType)
struct FAttributeChangeEvent
{
	GENERATED_USTRUCT_BODY()

public:
	FAttributeChangeEvent()
	{}

	FAttributeChangeEvent(FDamageEvent const& InDamageEvent)
		: DamageTypeClass(InDamageEvent.DamageTypeClass)
	{}

	explicit FAttributeChangeEvent(TSubclassOf<class UDamageType> InDamageTypeClass)
		: DamageTypeClass(InDamageTypeClass)
	{}

	/** Optional DamageType for this event.  If NULL, UDamageType will be assumed. */
	UPROPERTY()
		TSubclassOf<class UDamageType> DamageTypeClass;

	/** ID for this class. NOTE this must be unique for all damage events. */
	static const int32 ClassID = 0;

	virtual int32 GetTypeID() const { return FAttributeChangeEvent::ClassID; }
	virtual bool IsOfType(int32 InID) const { return FAttributeChangeEvent::ClassID == InID; };
};

USTRUCT(BlueprintType)
struct FPointAttributeChangeEvent : public FAttributeChangeEvent
{
	GENERATED_USTRUCT_BODY()
public:
		// skipping ImpulseMag for now
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attribute")
		FAttribute AttributeMod;

	/** Direction the shot came from. Should be normalized. */
	UPROPERTY()
		FVector_NetQuantizeNormal ShotDirection;

	UPROPERTY()
	struct FHitResult HitInfo;

	FPointAttributeChangeEvent() : HitInfo() {}
	FPointAttributeChangeEvent(FAttribute AttributeModIn, struct FHitResult const& InHitInfo, FVector const& InShotDirection, TSubclassOf<class UDamageType> InDamageTypeClass)
		: FAttributeChangeEvent(InDamageTypeClass), AttributeMod(AttributeModIn), ShotDirection(InShotDirection), HitInfo(InHitInfo)
	{}

	/** ID for this class. NOTE this must be unique for all damage events. */
	static const int32 ClassID = 1;

	virtual int32 GetTypeID() const { return FPointAttributeChangeEvent::ClassID; };
	virtual bool IsOfType(int32 InID) const { return (FPointAttributeChangeEvent::ClassID == InID) || FAttributeChangeEvent::IsOfType(InID); };
};

USTRUCT(BlueprintType)
struct FRadialAttributeChangeEvent : public FAttributeChangeEvent
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
		FRadialDamageParams Params;

	UPROPERTY()
		FVector Origin;

	// @fixme, will not replicate properly?  component pointer
	UPROPERTY()
		TArray<struct FHitResult> ComponentHits;

	/** ID for this class. NOTE this must be unique for all damage events. */
	static const int32 ClassID = 2;

	virtual int32 GetTypeID() const { return FRadialAttributeChangeEvent::ClassID; };
	virtual bool IsOfType(int32 InID) const { return (FRadialAttributeChangeEvent::ClassID == InID) || FAttributeChangeEvent::IsOfType(InID); };
};

USTRUCT(BlueprintType)
struct FHitInfo
{
	GENERATED_USTRUCT_BODY()
public:

};