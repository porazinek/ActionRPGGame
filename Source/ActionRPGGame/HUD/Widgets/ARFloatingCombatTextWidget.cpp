// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "ActionRPGGame.h"

#include "../../Types/ARStructTypes.h"
#include "../../ARPlayerController.h"
#include "../../Componenets/ARAttributeComponent.h"
#include "../../ARCharacter.h"

#include "../../Abilities/ARAbility.h"
#include "../../ActionState/ARActionStateComponent.h"

#include "RHI.h"

#include "ARFloatingCombatTextWidget.h"


void SARFloatingCombatTextWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;
	MyPC = InArgs._MyPC;
	MyAttrComp = InArgs._MyAttrComp;
	DisplayNumber = InArgs._DisplayNumber;
	TargetActor = InArgs._TargetActor;
	//MyAttrComp->OnInstigatorDamage.AddRaw(this, &SARFloatingCombatTextWidget::CreateDamageText);
	CurrentLifeTime = 0;
	//TargetActor = MyAttrComp->ChangedAttribute.DamageTarget;
	CurrentPosition.Bind(this, &SARFloatingCombatTextWidget::GetCurrentPosition);
	ChildSlot
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			[
				SNew(SCanvas)
				+ SCanvas::Slot()
				.Position(CurrentPosition)
				.Size(FVector2D(300, 300))
				[
					SNew(STextBlock)
					.ShadowColorAndOpacity(FLinearColor::Black)
					.ColorAndOpacity(FLinearColor::White)
					.ShadowOffset(FIntPoint(-1, 1))
					.Font(FSlateFontInfo("Veranda", 16))
					.Text(FText::FromString("Awesome FCT"))
				]
			]
			//SAssignNew(WidgetCanvas, SCanvas)
			//SAssignNew(TextBlock, SOverlay)
			//+ SOverlay::Slot()
			//[
			//	SNew(STextBlock)
			//	.ShadowColorAndOpacity(FLinearColor::Black)
			//	.ColorAndOpacity(FLinearColor::White)
			//	.ShadowOffset(FIntPoint(-1, 1))
			//	.Font(FSlateFontInfo("Veranda", 16))
			//	.Text(FText::AsNumber(DisplayNumber))
			//]
		];
}
FVector2D SARFloatingCombatTextWidget::GetCurrentPosition() const
{
	FVector2D ret = FVector2D::ZeroVector;
	if (TargetActor.IsValid())
	{
		ret = Projecte(TargetActor->GetActorLocation());
		ret.X = TargetActor->GetActorLocation().X;
		ret.Y = TargetActor->GetActorLocation().Y;
		return ret;
		
		
	}
	
	return ret;
}
FVector2D SARFloatingCombatTextWidget::Projecte(FVector VectorIn) const
{
	FVector2D ret = FVector2D::ZeroVector;
	FMatrix tempMatrix;
	FVector4 tempVec4 = tempMatrix.TransformFVector4(FVector4(VectorIn, 1));

	FPlane plane = tempVec4;
	if (plane.W == 0)
	{
		plane.W = KINDA_SMALL_NUMBER;
	}

	const float RHW = 1.0f / plane.W;
	
	FPlane FinalPlane = FPlane(plane.X * RHW, plane.Y * RHW, plane.Z*RHW, plane.W);

	FPlane V(0, 0, 0, 0);
	V = FinalPlane;

	FVector ResultVec(V);
	int32 ScreenX, ScreenY;
	MyPC->GetViewportSize(ScreenX, ScreenY);
	
	ResultVec.X = (ScreenX / 2.f) + (ResultVec.X*(ScreenX / 2.f));
	//ResultVec.Y *= -1.f * GProjectionSignY;
	ResultVec.Y = (ScreenY / 2.f) + (ResultVec.Y*(ScreenY / 2.f));

	if (V.W <= 0.0f)
	{
		ResultVec.Z = 0.0f;
	}
	ret.X = ResultVec.X;
	ret.Y = ResultVec.Y;
	return ret;
}
void SARFloatingCombatTextWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
	CurrentLifeTime += InDeltaTime;
	if (InDeltaTime >= 1)
	{
		TextBlock.Reset();
	}
}

void SARFloatingCombatTextWidget::CreateDamageText()
{
	//if (TextBlock.IsValid())
	//{
	//	TextBlock->SetText(FText::AsNumber(MyAttrComp->UIDamage.Value));
	//}
}
//
//int32 SARFloatingCombatTextWidget::OnPaint(const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
//{
//	const FSlateBrush* BrushResource = new FSlateBrush();
//
//	FSlateDrawElement::MakeBox(
//		OutDrawElements,
//		LayerId,
//		AllottedGeometry.ToPaintGeometry(),
//		BrushResource,
//		MyClippingRect,
//		ESlateDrawEffect::None,
//		FLinearColor::Gray * 0.35f
//		);
//	if (MyAttrComp.IsValid())
//	{
//				FVector2D WidgetSize = MyClippingRect.GetSize();
//				float HealthWidth = (MyAttrComp->Energy * WidgetSize.X) / MyAttrComp->MaxEnergy;
//
//				// Draw current health
//				FSlateDrawElement::MakeBox(
//					OutDrawElements,
//					LayerId,
//					AllottedGeometry.ToPaintGeometry(FVector2D::ZeroVector, FVector2D(HealthWidth, WidgetSize.Y)),
//					BrushResource,
//					MyClippingRect,
//					ESlateDrawEffect::None,
//					FLinearColor::Blue *.9f
//					);
//	}
//
//	return SCompoundWidget::OnPaint(AllottedGeometry, MyClippingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
//}
