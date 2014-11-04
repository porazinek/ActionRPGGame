// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../ARHUD.h"

#include "../../Types/ARStructTypes.h"

#include "SlateBasics.h"

class SARResourceBarWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SARResourceBarWidget)
	{}
	/*See private declaration of OwnerHUD below.*/
	SLATE_ARGUMENT(TWeakObjectPtr<class AARHUD>, OwnerHUD)

		SLATE_ARGUMENT(TWeakObjectPtr<class AARPlayerController>, MyPC)

		SLATE_ARGUMENT(TWeakObjectPtr<class UARAttributeComponent>, MyAttrComp)

		SLATE_ATTRIBUTE(float, CurrentValue)

		SLATE_ATTRIBUTE(float, MaxValue)
	SLATE_END_ARGS()

public:
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/////Needed for every widget
	/////Builds this widget and any of it's children
	void Construct(const FArguments& InArgs);

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	TAttribute<float> CurrentValue;
	TAttribute<float> MaxValue;
private:
	TWeakObjectPtr<class AARHUD> OwnerHUD;

	TWeakObjectPtr<class AARPlayerController> MyPC;

	TWeakObjectPtr<class AARCharacter> MyChar;

	TWeakObjectPtr<class UARAttributeComponent> MyAttrComp;
};



