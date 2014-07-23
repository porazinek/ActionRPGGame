// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "ActionRPGGame.h"

#include "../../Types/ARStructTypes.h"
#include "../../ARPlayerController.h"

#include "../../Componenets/ARAttributeComponent.h"
#include "../../ARCharacter.h"

#include "../../Abilities/ARAbility.h"
#include "../../ActionState/ARActionStateComponent.h"

#include "ARHealthBarWidget.h"


void SARHealthBarWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;
	MyPC = InArgs._MyPC;
	MyAttrComp = InArgs._MyAttrComp;
	CurrentValue = InArgs._CurrentValue;
	MaxValue = InArgs._MaxValue;
	//ChildSlot
	//	[
	//		SNew(SOverlay)
	//		+ SOverlay::Slot()
	//		[
	//			SNew(STextBlock)
	//			.ShadowColorAndOpacity(FLinearColor::Black)
	//			.ColorAndOpacity(FLinearColor::White)
	//			.ShadowOffset(FIntPoint(-1, 1))
	//			.Font(FSlateFontInfo("Veranda", 16))
	//			.Text(this, &SARCastingBarWidget::GetCurrentCastTime)
	//		]
	//	];
}

int32 SARHealthBarWidget::OnPaint(const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
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
		//float HealthWidth = (MyAttrComp->Health * WidgetSize.X) / MyAttrComp->MaxHealth;
		float HealthWidth = (CurrentValue.Get() * WidgetSize.X) / MaxValue.Get();
		// Draw current health
		FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(FVector2D::ZeroVector, FVector2D(HealthWidth, WidgetSize.Y)),
			BrushResource,
			MyClippingRect,
			ESlateDrawEffect::None,
			FLinearColor::Red *.9f
			);
	}

	return SCompoundWidget::OnPaint(AllottedGeometry, MyClippingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}