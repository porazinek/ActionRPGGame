// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../Types/ARStructTypes.h"
#include "../Types/AREnumTypes.h"

#include "ARAttributeBaseComponent.generated.h"
//dumb delegates declarations
//these will need to transfer some properties around.

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeModified, FAttributeModified, ModifiedAttribute);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChanged, FAttributeChanged, AttributeChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInstigatorCausedDamage, FAttributeChanged, AttributeChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_EightParams(FOnPointAttributeChange, FAttribute, Attribute, class AActor*, InstigatedBy, FVector, HitLocation, class UPrimitiveComponent*, FHitComponent, FName, BoneName, FVector, ShotFromDirection, const class UDamageType*, DamageType, class AActor*, DamageCauser);
/*
	Despite the name, AttributeBaseComponent DO NOT hold any attributes.
	Attributes should be defined in component derived from this class.

	You can change it hold attributes in seprate class, but keeping it in single
	inheritance hierachy makes things much easier.

	Access to attribute properties is done trough Unreal code reflection system.
	*/

//forward declarations
class AAREffectPeriodic;

UCLASS(hidecategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming), editinlinenew, meta = (BlueprintSpawnableComponent))
class UARAttributeBaseComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UPROPERTY(Replicated, RepRetry, BlueprintReadOnly, Category = "Effect")
		FActivePeriodicEffects ActivePeriodicEffects;

	//these exist only on client.
	UPROPERTY(BlueprintReadOnly, Category = "Effect")
		TArray<FPeriodicEffect> ActiveClientEffects;

	void AddPeriodicEffect(FPeriodicEffect PeriodicEffect);

	void RemovePeriodicEffect(class AAREffectPeriodic* PeriodicEffect);

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Attribute")
		FOnAttributeModified OnAttributeModified;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Attribute")
		FOnAttributeChanged OnAttributeChanged;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Attribute")
		FOnPointAttributeChange OnPointAttributeChange;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Attribute")
		FOnInstigatorCausedDamage OnInstigatorCausedDamage;
public:
	/* Get Attribute (UProperty), from component class */
	UProperty* GetAttribute(FName AttributeName);

	/* Get float value is Attribute (UProperty) */
	float GetFloatValue(FName AttributeName);

	/* Set float value is Attribute (UProperty) */
	void SetFloatValue(float InValue, FName AttributeName);

public:
	/*
		Final point of changing attributes. All attribute changes should be at some point routed
		to this function.
		This function shouldn't ever implement things like armor reduction or other math formulas.
		That is what Effects are for. Or weapons. Or whatever.
	*/
	virtual void DamageAttribute(FARDamageEvent const& DamageEvent, AActor* EventInstigator, AActor* DamageCauser);

	virtual void HealAttribute();
	virtual void ChangeAttribute(FName AttributeName, float ModValue, TEnumAsByte<EAttrOp> OperationType);
	
	//virtual void ChangeAttribute(FAttributeChangeEvent const& AttributeEvent, AController* EventInstigator, AActor* DamageCauser);

	virtual void InstigatedAttributeChange(FAttribute Attribute, AActor* DamageTarget, AActor* DamageCauser, AActor* Instigator, UDamageType* DamageType);

	virtual void SetAttributeChange(FAttribute Attribute, AActor* DamageTarget, AActor* DamageCauser, AActor* Instigator, UDamageType* DamageType);

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Attribute")
		FAttributeChanged ChangedAttribute;

	//UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Attribute")
	//	FOnAttributeChanged OnAttributeChanged;

protected:
	void SetAttributeModified(float ModValue, FName AttributeName);
	void GetAttributeModified();
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ModifiedAttribute, Category = "Attributes")
		FAttributeModified ModifiedAttribute;
	UFUNCTION()
		void OnRep_ModifiedAttribute();

private:
	float AttributeOp(float ModValue, float AttrValueIn, TEnumAsByte<EAttrOp> OpType);

public:
	/* Get unsigned integer value is Attribute (UProperty) */
	int32 GetIntValue(FName AttributeName);

	/* Set unsigned integer value is Attribute (UProperty) */
	void SetIntValue(int32 InValue, FName AttributeName);

};



