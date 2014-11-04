// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../ARHUD.h"

#include "../../Types/ARStructTypes.h"

#include "SlateBasics.h"

class SARTargetInfoWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SARTargetInfoWidget)
	{}
	/*See private declaration of OwnerHUD below.*/
	SLATE_ARGUMENT(TWeakObjectPtr<class AARHUD>, OwnerHUD)

		SLATE_ARGUMENT(TWeakObjectPtr<class AARPlayerController>, MyPC)

		SLATE_ATTRIBUTE(TWeakObjectPtr<class UARAttributeComponent>, MyAttrComp)

		SLATE_ARGUMENT(float, HealthBarWidth)

		SLATE_ARGUMENT(float, HealthBarHeight)

		SLATE_ARGUMENT(float, HealthBarPosX)

		SLATE_ARGUMENT(float, HealthBarPosY)

	SLATE_END_ARGS()

public:
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/////Needed for every widget
	/////Builds this widget and any of it's children
	void Construct(const FArguments& InArgs);

private:
	TWeakObjectPtr<class AARHUD> OwnerHUD;

	TWeakObjectPtr<class AARPlayerController> MyPC;

	TWeakObjectPtr<class AARCharacter> MyChar;

	TWeakObjectPtr<class UARAttributeComponent> MyAttrComp;

	float HealthBarWidth;

	float HealthBarHeight;

	float HealthBarPosX;

	float HealthBarPosY;

	TAttribute<FVector2D> HealthBarPosition;
	FVector2D GetHealthPosition() const;

	TAttribute<FVector2D> HealthBarSize;
	FVector2D GetHealthSize() const;

	TAttribute<float> CurrentHealth;
	TAttribute<float> MaxHealth;
	float GetCurrentHealth() const;
	float GetMaxHealth() const;
};



