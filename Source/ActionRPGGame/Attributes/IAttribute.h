// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../Types/ARAttributeTypes.h"
#include "IAttribute.generated.h"

/*
	Interface for handling attributes. Death of attribute owner for now.
*/

UINTERFACE()
class UIAttribute : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IIAttribute
{
	GENERATED_IINTERFACE_BODY()
public:
	UFUNCTION()
		virtual void Died();

	inline FName GetDeathAttribute(){ return DeathAttribute; };
	inline void SetDeathAttribute(FName ValueIn) { DeathAttribute = ValueIn; };

	/*[Server]*/
	UFUNCTION(BlueprintNativeEvent, BlueprintAuthorityOnly, Category = "Attribute")
		void OnRecivedDamage(FAttributeChanged AttributeChanged, FGameplayTagContainer DamageTag);
	UFUNCTION(BlueprintNativeEvent, BlueprintAuthorityOnly, Category = "Attribute")
		void OnRecivedRadialDamage(FAttributeChanged AttributeChanged, FARRadialDamageEvent const& Damage, FGameplayTagContainer DamageTag);

private:
	//name of attribute used for actor death check.
	//Maybe change to array so multiple attributes can be affected
	//and organize it into Struct so each attribute can have different death threshold.
	FName DeathAttribute;
};
