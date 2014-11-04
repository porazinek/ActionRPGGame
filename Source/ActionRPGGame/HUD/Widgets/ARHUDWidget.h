// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../ARHUD.h"

#include "../../Types/ARStructTypes.h"

#include "SlateBasics.h"

class SARHUDWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SARHUDWidget)
	{}
	/*See private declaration of OwnerHUD below.*/
		SLATE_ARGUMENT(TWeakObjectPtr<class AARHUD>, OwnerHUD)

		SLATE_ARGUMENT(TWeakObjectPtr<class AARPlayerController>, MyPC)

		SLATE_ARGUMENT(TWeakObjectPtr<class AARCharacter>, MyChar)

		SLATE_ARGUMENT(TWeakObjectPtr<class UARAttributeComponent>, MyAttrComp)

		SLATE_ATTRIBUTE(TWeakObjectPtr<class UARAttributeComponent>, TargetAttrComp)

		SLATE_ARGUMENT(float, CastbarWidth)

		SLATE_ARGUMENT(float, CastbarHeight)

		SLATE_ARGUMENT(float, CastbarPositionX)

		SLATE_ARGUMENT(float, CastbarPositionY)
	SLATE_END_ARGS()

public:
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/////Needed for every widget
	/////Builds this widget and any of it's children
	void Construct(const FArguments& InArgs);
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	void SetTargetResourceVisibility(EVisibility ValueIn);
	TAttribute < TWeakObjectPtr<class UARAttributeComponent> > TargetAttrComp;
private:

	EVisibility GetInventoryVisibility() const;
	EVisibility GetAbilityInventoryVisibility() const;
	EVisibility GetCharacterSheetVisibility() const;
	TWeakObjectPtr<class AARHUD> OwnerHUD;

	TWeakObjectPtr<class AARPlayerController> MyPC;

	TWeakObjectPtr<class AARPlayerController> TargetPC;

	TWeakObjectPtr<class AARCharacter> MyChar;

	TWeakObjectPtr<class UARAttributeComponent> MyAttrComp;

	TWeakObjectPtr<class UARAttributeComponent> GetTargetAttrComp() const;

	EVisibility GetTargetResourceVisibility() const;
	EVisibility TargetResourceVisibility;

	float CastbarWidth;
	float CastbarHeight;
	float CastbarPositionX;
	float CastbarPositionY;
};



