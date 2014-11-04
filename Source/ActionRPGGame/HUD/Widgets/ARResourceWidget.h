// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../ARHUD.h"

#include "../../Types/ARStructTypes.h"

#include "SlateBasics.h"

class SARResourceWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SARResourceWidget)
	{}
	/*See private declaration of OwnerHUD below.*/
		SLATE_ARGUMENT(TWeakObjectPtr<class AARHUD>, OwnerHUD)

		SLATE_ARGUMENT(TWeakObjectPtr<class AARPlayerController>, MyPC)

		SLATE_ARGUMENT(TWeakObjectPtr<class UARAttributeComponent>, MyAttrComp)
	SLATE_END_ARGS()

public:
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/////Needed for every widget
	/////Builds this widget and any of it's children
	void Construct(const FArguments& InArgs);
private:
	TWeakObjectPtr<class AARHUD> OwnerHUD;

	TWeakObjectPtr<class AARPlayerController> MyPC;

	TWeakObjectPtr<class UARAttributeComponent> MyAttrComp;

	TAttribute<float> CurrentHealth;
	TAttribute<float> MaxHealth;
	float GetCurrentHealth() const;
	float GetMaxHealth() const;

	TAttribute<float> CurrentEnergy;
	TAttribute<float> MaxEnergy;
	float GetCurrentEnergy() const;
	float GetMaxEnergy() const;

	TAttribute<float> CurrentStamina;
	TAttribute<float> MaxStamina;
	float GetCurrentStamina() const;
	float GetMaxStamina() const;
};



