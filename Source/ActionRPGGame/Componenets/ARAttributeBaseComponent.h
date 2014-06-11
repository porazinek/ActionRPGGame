// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../Types/ARStructTypes.h"
#include "../Types/AREnumTypes.h"

#include "ARAttributeBaseComponent.generated.h"
//dumb delegates declarations
//these will need to transfer some properties around.

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeModified, FAttributeModified, ModifiedAttribute);

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

		virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) OVERRIDE;

	UPROPERTY(Replicated, RepRetry, BlueprintReadOnly, Category = "Effect")
		FActivePeriodicEffects ActivePeriodicEffects;

	//these exist only on client.
	UPROPERTY(BlueprintReadOnly, Category = "Effect")
		TArray<FPeriodicEffect> ActiveClientEffects;

	void AddPeriodicEffect(FPeriodicEffect PeriodicEffect);

	void RemovePeriodicEffect(class AAREffectPeriodic* PeriodicEffect);

		UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Attribute")
			FOnAttributeModified OnAttributeModified;
	
protected:
	/* [Server] - I mean you really should call it ony server */
	/* Get Attribute (UProperty), from component class */
	UProperty* GetAttribute(FName AttributeName);

	/* Get float value is Attribute (UProperty) */
	float GetFloatValue(FName AttributeName);

	/* Set float value is Attribute (UProperty) */
	void SetFloatValue(float InValue, FName AttributeName);

public:
	void ChangeAttribute(float ModValue, FName AttributeName, TEnumAsByte<EAttrOp> OpType);
protected:
	void SetAttributeModified(float ModValue, FName AttributeName);
	void SetAttributeModifiedTwo();
	UPROPERTY(Replicated)
	FAttributeModified ModifiedAttribute;

private:
	float AttributeOp(float ModValue, float AttrValueIn, TEnumAsByte<EAttrOp> OpType);

public:
	/* Get unsigned integer value is Attribute (UProperty) */
	int32 GetIntValue(FName AttributeName);

	/* Set unsigned integer value is Attribute (UProperty) */
	void SetIntValue(int32 InValue, FName AttributeName);

};



