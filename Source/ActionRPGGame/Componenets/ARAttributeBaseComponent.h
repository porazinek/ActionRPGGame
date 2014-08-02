// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../Types/ARStructTypes.h"
#include "../Types/AREffectTypes.h"
#include "../Types/AREnumTypes.h"
#include "../Types/ARAttributeTypes.h"

#include "Slate.h"
#include "SlateCore.h"

#include "ARAttributeBaseComponent.generated.h"
//dumb delegates declarations
//these will need to transfer some properties around.

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeDamage, FAttributeChanged, AttributeChanged, FGameplayTagContainer, DamageTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInstigatorCausedDamage, FAttributeChanged, AttributeChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_EightParams(FOnPointAttributeDamage, FAttribute, Attribute, class AActor*, InstigatedBy, FVector, HitLocation, class UPrimitiveComponent*, FHitComponent, FName, BoneName, FVector, ShotFromDirection, const class UDamageType*, DamageType, class AActor*, DamageCauser);

DECLARE_MULTICAST_DELEGATE_OneParam(FDMDOnInstigatorDamage, FARUIDamage);
/*
	Event called when this component receives any damage
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDMDOnIncomingDamage, const FGameplayTagContainer&, DamageTags, const FAttribute&, Attribute);
/*
	Event called, when instigator (causer), caused any damage. So it is called from component owned
	by Instigator/Causer.
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDMDOnOutgoingDamage, const FGameplayTagContainer&, DamageTags, const FAttribute&, Attribute);


/* stubs */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPeriodicEffectAppiled, FGameplayTagContainer, OwnedTags);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPeriodicEffectRemoved, FGameplayTagContainer, OwnedTags);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPeriodicEffectInstigated, const FGameplayTagContainer&, DamageTag);
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
	
	/*
		Called after properties are set and replicated. From actor which owns component.
	*/
	virtual void Initialize();
	
	/**
		Common Properties
	**/
	/*
		Controller From actor who owns this component.
	*/
	UPROPERTY()
	class AARPlayerController* PlayerController;

	UPROPERTY()
	class AARCharacter* PlayerCharacter;
	/**
		Effects
	**/
	/*
		these needs to be cleaned up. Periodic effects are to be redone.
	*/
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

	UFUNCTION(NetMulticast, Unreliable)
		void AttachEffectCue(FEffectSpec EffectIn);
	UFUNCTION(NetMulticast, Unreliable)
		void DetachEffectCue(FEffectSpec EffectIn);


	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRemovePeriodicEffect(class AAREffectPeriodic* PeriodicEffect);

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
		Broadcast tags owned by this effect.
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Attribute")
		FOnPeriodicEffectRemoved OnPeriodicEffectRemoved;

	/**
		Attribute Handling
	**/
	/*
		Called when any damage to attribute is appilied.
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Attribute")
		FOnAttributeDamage OnAttributeDamage;
	/*
		Called when point damage to attribute is appilied.
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Attribute")
		FOnPointAttributeDamage OnPointAttributeDamage;
	/*
		Called on instigator of damage. PROBABLY REMOVE IT!
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Attribute")
		FOnInstigatorCausedDamage OnInstigatorCausedDamage;

	/*
		This for UI. PROBABLY REMOVE IT.
	*/
	FDMDOnInstigatorDamage OnInstigatorDamage;

	/*
		We need two Delegate Events OnOutgoingDamage and OnIncomingDamage, becasue if we only had one
		all events would be fired up, and would modify Damage on Instigator and Target.
		It would create strange situation, where Armor Effect on Instigator, would reduce damage
		taken by Target.

		This system still needs more fleshing out, and testing.
	*/
	/*
		Called when damage is incoming to this component.
		Ie. it's going to change attribute on this component.
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Attribute")
		FDMDOnIncomingDamage OnIncomingDamage;
	/*
		In case of my implementation it calls SetAttributeForMod, on damage instigator.
		So any effects/mods/etc. present on instigator have chance to modify damage,
		before it's going to appilied on target.
	*/
	/*
		Damage is going from instigator component. 
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Attribute")
		FDMDOnOutgoingDamage OnOutgoingDamage;
	/*
	
	*/
	UFUNCTION()
		void SetAttributeForMod(const FGameplayTagContainer& DamageTags, const FAttribute& Attribute);
	/*
		Store current damage value. In conjuction with SetAttributeForMod and SetFinalDmage.
	*/
	FAttribute CachedAttribute;

	/*
		Helper function. When Effect modify damage, this function should be called,
		and it will set new damage to CachedAttribute;
	*/
	UFUNCTION(BlueprintCallable, Category = "AR|Attribute Dmage")
		void SetFinalDmage(const FAttribute& AttributeIn);
private:
	void SetDamageReplication(UARAttributeBaseComponent* Comp);
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
	void ChangeAttribute(FName AttributeName, float ModValue, TEnumAsByte<EAttrOp> OperationType);
	
	/*
		AttributeA > AttributeB
	*/
	bool CompareAttributes(FName AttributeA, FName AttributeB, TEnumAsByte<ECompareAttribute::Type> Comparsion);
	bool CompareAttributeValue(float Value, FName Attribute, TEnumAsByte<ECompareAttribute::Type> Comparsion);
	//virtual void ChangeAttribute(FAttributeChangeEvent const& AttributeEvent, AController* EventInstigator, AActor* DamageCauser);

	/*
		Damage cause to attribute by Instigator. For example when Player1 deal Damage to Player2,
		the Player1 will usually be instigator. Though it can be any object with Attribute
		Component Attached.
	*/
	virtual void InstigatorAttributeDamageCaused(FAttribute Attribute, FVector HitLocation, AActor* DamageTarget, AActor* DamageCauser, AActor* Instigator, UDamageType* DamageType, FGameplayTagContainer DamageTag);

	virtual void SetAttributeChange(FAttribute Attribute, FVector HitLocation, AActor* DamageTarget, AActor* DamageCauser, AActor* Instigator, UDamageType* DamageType, FGameplayTagContainer DamageTag);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_InstigatorDamageCaused, Category = "Attribute")
		FAttributeChanged ChangedAttribute;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_UIDamage, Category = "UI")
		FARUIDamage UIDamage;
	UFUNCTION()
		void OnRep_UIDamage();
	UFUNCTION()
		void OnRep_InstigatorDamageCaused();

protected:
	void SetAttributeModified(float ModValue, FName AttributeName);
	void GetAttributeModified();
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ModifiedAttribute, Category = "Attributes")
		FAttributeModified ModifiedAttribute;
	UFUNCTION()
		void OnRep_ModifiedAttribute();

private:
	float AttributeOp(float ModValue, float AttrValueIn, TEnumAsByte<EAttrOp> OpType);


	/*
		Some Slate crap for testing -;-
	*/
	
public:
	/* Get unsigned integer value is Attribute (UProperty) */
	int32 GetIntValue(FName AttributeName);

	/* Set unsigned integer value is Attribute (UProperty) */
	void SetIntValue(int32 InValue, FName AttributeName);

};



