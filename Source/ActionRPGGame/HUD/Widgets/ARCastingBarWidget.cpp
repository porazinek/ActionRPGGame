// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "ActionRPGGame.h"

#include "../../Types/ARStructTypes.h"
#include "../../ARPlayerController.h"

#include "../../Abilities/ARAbility.h"
#include "../../ActionState/ARActionStateComponent.h"
#include "../../Abilities/ARAbilityComponent.h"

#include "ARCastingBarWidget.h"


void SARCastingBarWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;
	MyPC = InArgs._MyPC;
	AbilityComp = InArgs._AbilityComp;
	MaxValue = InArgs._MaxValue.Get();
	CurrentValue = InArgs._CurrentValue.Get();
	OnGetFloat = InArgs._OnGetFloat;
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

int32 SARCastingBarWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
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
	if (AbilityComp.IsValid())
	{
		if (AbilityComp->ActiveAbility)
		{
			//if (MyPC->ActiveAbility->OnGetFloat.IsBound())
			//{
				FVector2D WidgetSize = MyClippingRect.GetSize();
				float HealthWidth = (AbilityComp->ActiveAbility->CurrentCastTime * WidgetSize.X) / AbilityComp->ActiveAbility->MaxCastTime;

				// Draw current health
				FSlateDrawElement::MakeBox(
					OutDrawElements,
					LayerId,
					AllottedGeometry.ToPaintGeometry(FVector2D::ZeroVector, FVector2D(HealthWidth, WidgetSize.Y)),
					BrushResource,
					MyClippingRect,
					ESlateDrawEffect::None,
					FLinearColor::Green *.65f
					);
			//}
		}
	}

	return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyClippingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}

/*
	For now.
	Task:
	1. Create an actual graphical bar ;).
	2. Hide it when nothing is casted.
	3. Show only when ability is casting.
	*/
FText SARCastingBarWidget::GetCurrentCastTime() const
{
	if (MyPC.IsValid())
	{
		if (MyPC->ActiveAbility)
		{
			return FText::AsNumber(MyPC->ActiveAbility->CurrentCastTime);
		}
	}
	return FText::FromString("0");
}