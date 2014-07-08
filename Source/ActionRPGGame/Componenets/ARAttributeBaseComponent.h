// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../Types/ARStructTypes.h"
#include "../Types/AREffectTypes.h"
#include "../Types/AREnumTypes.h"

#include "ARAttributeBaseComponent.generated.h"
//dumb delegates declarations
//these will need to transfer some properties around.

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeDamage, FAttributeChanged, AttributeChanged, FGameplayTagContainer, DamageTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInstigatorCausedDamage, FAttributeChanged, AttributeChanged, FGameplayTagContainer, DamageTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_EightParams(FOnPointAttributeDamage, FAttribute, Attribute, class AActor*, InstigatedBy, FVector, HitLocation, class UPrimitiveComponent*, FHitComponent, FName, BoneName, FVector, ShotFromDirection, const class UDamageType*, DamageType, class AActor*, DamageCauser);

/* stubs */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPeriodicEffectAppiled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPeriodicEffectRemoved);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPeriodicEffectInstigated);
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

	UPROPERTY(ReplicatedUsing=OnRep_EffectAppiled, BlueprintReadOnly, Category="Effects")
		FActiveEffectList ActiveEffects;
	UFUNCTION()
		void OnRep_EffectAppiled();
	void ApplyPeriodicEffect(FEffectSpec& EffectIn);

	void RemovedPeriodicEffect_TEMP(UAREffectPeriodicO* EffectIn);

	//these exist only on client.
	UPROPERTY(BlueprintReadOnly, Category = "Effect")
		TArray<FPeriodicEffect> ActiveClientEffects;

	void AddPeriodicEffect(FEffectSpec& PeriodicEffect);

	void RemovePeriodicEffect(class AAREffectPeriodic* PeriodicEffect);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRemovePeriodicEffect(class AAREffectPeriodic* PeriodicEffect);

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Attribute")
		FOnAttributeDamage OnAttributeDamage;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Attribute")
		FOnPointAttributeDamage OnPointAttributeDamage;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Attribute")
		FOnInstigatorCausedDamage OnInstigatorCausedDamage;

	/*
		Periodic Effect appiled By Me.
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Attribute")
		FOnPeriodicEffectInstigated OnPeriodicEffectInstigated;
	/*
		Periodic Effect Appiled to Me.
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Attribute")
		FOnPeriodicEffectAppiled OnPeriodicEffectAppiled;
	/*
		Periodic Effect Removed from Me.
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Attribute")
		FOnPeriodicEffectRemoved OnPeriodicEffectRemoved;


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
		It is important, because here we will call all events and set replicated properties.
	*/
	virtual void DamageAttribute(FARDamageEvent const& DamageEvent, AActor* EventInstigator, AActor* DamageCauser);

	virtual void HealAttribute();
	virtual void ChangeAttribute(FName AttributeName, float ModValue, TEnumAsByte<EAttrOp> OperationType);
	
	//virtual void ChangeAttribute(FAttributeChangeEvent const& AttributeEvent, AController* EventInstigator, AActor* DamageCauser);

	/*
		Damage cause to attribute by Instigator. For example when Player1 deal Damage to Player2,
		the Player1 will usually be instigator. Though it can be any object with Attribute
		Component Attached.
	*/
	virtual void InstigatorAttributeDamageCaused(FAttribute Attribute, AActor* DamageTarget, AActor* DamageCauser, AActor* Instigator, UDamageType* DamageType, FGameplayTagContainer DamageTag);

	virtual void SetAttributeChange(FAttribute Attribute, AActor* DamageTarget, AActor* DamageCauser, AActor* Instigator, UDamageType* DamageType, FGameplayTagContainer DamageTag);

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Attribute")
		FAttributeChanged ChangedAttribute;

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



