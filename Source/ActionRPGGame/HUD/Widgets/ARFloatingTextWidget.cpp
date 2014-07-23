// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "ActionRPGGame.h"

#include "../../Types/ARStructTypes.h"
#include "../../ARPlayerController.h"
#include "../../Componenets/ARAttributeComponent.h"
#include "../../ARCharacter.h"

#include "../../Abilities/ARAbility.h"
#include "../../ActionState/ARActionStateComponent.h"

#include "ARFloatingTextWidget.h"


void SARFloatingTextWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;
	MyPC = InArgs._MyPC;
	MyAttrComp = InArgs._MyAttrComp;
}

int32 SARFloatingTextWidget::OnPaint(const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const FSlateBrush* BrushResource = new FSlateBrush();
	
	FSlateDrawElement::MakeBox(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(),
		BrushResource,
		MyClippingRect,
		ESlateDrawEffect::None,
		FLinearColor::Gray * 0.35f
		);
	if (MyAttrComp.IsValid())
	{
				FVector2D WidgetSize = MyClippingRect.GetSize();
				float HealthWidth = (MyAttrComp->Energy * WidgetSize.X) / MyAttrComp->MaxEnergy;

				// Draw current health
				FSlateDrawElement::MakeBox(
					OutDrawElements,
					LayerId,
					AllottedGeometry.ToPaintGeometry(FVector2D::ZeroVector, FVector2D(HealthWidth, WidgetSize.Y)),
					BrushResource,
					MyClippingRect,
					ESlateDrawEffect::None,
					FLinearColor::Blue *.9f
					);
	}

	return SCompoundWidget::OnPaint(AllottedGeometry, MyClippingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}
