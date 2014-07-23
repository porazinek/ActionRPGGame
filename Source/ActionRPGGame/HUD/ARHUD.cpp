// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "Widgets/ARButtonWidget.h"
#include "Widgets/ARHUDWidget.h"
#include "Widgets/ARResourceWidget.h"
#include "Widgets/ARTargetInfoWidget.h"

#include "../ARCharacter.h"
#include "../ARPlayerController.h"
#include "../Componenets/AREquipmentComponent.h"
#include "../Componenets/ARAttributeComponent.h"

#include "../BlueprintLibrary/ARTraceStatics.h"

#include "ARHUD.h"

AARHUD::AARHUD(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("Texture2D'/Game/SharedAssets/HUD/crosshair.crosshair'"));
	CrosshairTex = CrosshairTexObj.Object;

	DrawInventoryWidget = false;
	FCTPosition = FVector2D::ZeroVector;
	SCTLifeTime = 0.4;
	CurrentSCTLifeTime = 0;


	DamageIndicators.AddZeroed(10);
}


void AARHUD::DrawHUD()
{
	Super::DrawHUD();

	// find center of the screen/Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	if (CrosshairTex && Canvas)
	{
		FVector2D CrosshairDrawStart(Center);
		CrosshairDrawStart.X -= CrosshairTex->GetSurfaceWidth() * 0.5f;
		CrosshairDrawStart.Y -= CrosshairTex->GetSurfaceHeight() * 0.5f;

		FCanvasTileItem TileItem(CrosshairDrawStart, CrosshairTex->Resource, FLinearColor::Red);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}
	if (!HUDWidget.IsValid() && GetOwningPawn())
	{
		AARPlayerController* MyPC = Cast<AARPlayerController>(GetOwningPlayerController());
		//null on BeginPlay
		AARCharacter* Character = Cast<AARCharacter>(GetOwningPawn());
		OwnerChar = Character;
		//TWeakObjectPtr<UARAttributeComponent> MyAttr = Character->Attributes.Get();
		if (MyPC)
		{
			Character = Cast<AARCharacter>(MyPC->GetPawn());
		}
		SAssignNew(HUDWidget, SARHUDWidget).OwnerHUD(this).IsEnabled(true).MyPC(MyPC).MyChar(Character).MyAttrComp(Character->Attributes.Get()).CastbarHeight(CastbarHeight)
			.CastbarWidth(CastbarWidth).CastbarPositionX(Canvas->ClipX*CastbarPositionX).CastbarPositionY(Canvas->ClipY*CastbarPositionY);



		////////////////////////////////////////////////////////////////////////////////////////////////////
		///Pass our viewport a weak ptr to our widget
		if (GEngine->IsValidLowLevel())
		{
			GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(HUDWidget.ToSharedRef()));
			/*Viewport's weak ptr will not give Viewport ownership of Widget*/
		}

		if (HUDWidget.IsValid())
		{
			//////////////////////////////////////////////////////////////////////////////////////////////////
			///Set widget's properties as visible (sets child widget's properties recursively)
			HUDWidget->SetVisibility(EVisibility::Visible);
			HUDWidget->FCTPosition.BindUObject(this, &AARHUD::GetFCTPosition);
			HUDWidget->FCTVisibility.BindUObject(this, &AARHUD::GetFCTVisibility);
		}
	}

	DrawResourceWidgetForTarget();
	if (OwnerChar.IsValid())
	{
		if (Canvas)
		{
			//FVector temp = Canvas->Project(OwnerChar->Attributes->ChangedAttribute.DamageTarget->GetActorLocation());
			FVector temp = Canvas->Project(OwnerChar->Attributes->UIDamage.Location);
			FCTPosition.X = temp.X;
			FCTPosition.Y = temp.Y;
		}
	}
}

void AARHUD::PawnDamaged(FVector HitLocation, float DamageAmount, TSubclassOf<UDamageType> DamageClass)
{
	// Calculate the rotation 	
	//AUTCharacter* UTC = UTPlayerOwner->GetUTCharacter();
	if (OwnerChar.IsValid())	// If have a pawn and it's alive...
	{
		FVector CharacterLocation;
		FRotator CharacterRotation;

		OwnerChar->GetActorEyesViewPoint(CharacterLocation, CharacterRotation);
		FVector HitSafeNormal = (HitLocation - CharacterLocation).SafeNormal2D();
		float Ang = FMath::Acos(FVector::DotProduct(CharacterRotation.Vector().SafeNormal2D(), HitSafeNormal)) * (180.0f / PI);

		// Figure out Left/Right....
		float FinalAng = (FVector::DotProduct(FVector::CrossProduct(CharacterRotation.Vector(), FVector(0, 0, 1)), HitSafeNormal)) > 0 ? 360 - Ang : Ang;

		int BestIndex = 0;
		float BestTime = DamageIndicators[0].FadeTime;
		for (int i = 0; i < 10; i++)
		{
			if (DamageIndicators[i].FadeTime <= 0.0f)
			{
				BestIndex = i;
				break;
			}
			else
			{
				if (DamageIndicators[i].FadeTime < BestTime)
				{
					BestIndex = i;
					BestTime = DamageIndicators[i].FadeTime;
				}
			}
		}

		DamageIndicators[BestIndex].FadeTime = DAMAGE_FADE_DURATION;
		DamageIndicators[BestIndex].RotationAngle = FinalAng;
		DamageIndicators[BestIndex].DamageAmount = DamageAmount;
		//		UUTHUDWidget_WeaponCrosshair* CrossHairWidget =


	}
}

void AARHUD::DrawDamageIndicators()
{
	FLinearColor DrawColor = FLinearColor::White;
	for (int i = 0; i < DamageIndicators.Num(); i++)
	{
		if (DamageIndicators[i].FadeTime > 0.0f)
		{
			DrawColor.A = 1.0 * (DamageIndicators[i].FadeTime / DAMAGE_FADE_DURATION);

			float Size = 384 * (Canvas->ClipY / 720.0f);
			float Half = Size * 0.5;
			FCanvasTextItem TextItem;
			TextItem.Text = FText::AsNumber(DamageIndicators[i].DamageAmount);
			TextItem.Scale = 2;
			TextItem.Position = FVector2D((Canvas->ClipX * 0.5) - Half, (Canvas->ClipY * 0.5) - Half);
			//FCanvasTileItem ImageItem(FVector2D((Canvas->ClipX * 0.5) - Half, (Canvas->ClipY * 0.5) - Half), DamageIndicatorTexture->Resource, FVector2D(Size, Size), FVector2D(0, 0), FVector2D(1, 1), DrawColor);
			//ImageItem.Rotation = FRotator(0, DamageIndicators[i].RotationAngle, 0);
			//ImageItem.PivotPoint = FVector2D(0.5, 0.5);
			//ImageItem.BlendMode = ESimpleElementBlendMode::SE_BLEND_Translucent;
			Canvas->DrawItem(TextItem);

			DamageIndicators[i].FadeTime -= RenderDelta;
		}
	}
}


FVector2D AARHUD::GetFCTPosition() const
{
	return FCTPosition;
}
EVisibility AARHUD::GetFCTVisibility() const
{
	if (OwnerChar.IsValid())
	{
		if (OwnerChar->Attributes->ChangedAttribute.DamageTarget)
		{
			return EVisibility::Visible;
		}
	}
	return EVisibility::Collapsed;
}
void AARHUD::BeginPlay()
{
	//ScreenSizeX = Canvas->ClipX;
	//ScreenSizeY = Canvas->ClipY;

	//AARPlayerController* MyPC = Cast<AARPlayerController>(GetOwningPlayerController());
	////null on BeginPlay
	//AARCharacter* Character = NULL;
	//if (MyPC)
	//{
	//	Character = Cast<AARCharacter>(MyPC->GetPawn());
	//}
	//SAssignNew(HUDWidget, SARHUDWidget).OwnerHUD(this).IsEnabled(true).MyPC(MyPC).CastbarHeight(CastbarHeight)
	//	.CastbarWidth(CastbarWidth).CastbarPositionX(CastbarPositionX).CastbarPositionY(CastbarPositionY);
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	/////Pass our viewport a weak ptr to our widget
	//if (GEngine->IsValidLowLevel())
	//{
	//	GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(HUDWidget.ToSharedRef()));
	//		/*Viewport's weak ptr will not give Viewport ownership of Widget*/
	//}

	//if (HUDWidget.IsValid())
	//{
	//	//////////////////////////////////////////////////////////////////////////////////////////////////
	//	///Set widget's properties as visible (sets child widget's properties recursively)
	//	HUDWidget->SetVisibility(EVisibility::Visible);
	//}
}
void AARHUD::DrawResourceBars()
{

}

void AARHUD::DrawResourceWidgetForTarget()
{
	if (!GetOwningPawn())
		return;

	FHitResult HitResult = UARTraceStatics::GetHitResult(10000, NAME_None, GetOwningPawn(), false, false, EARTraceType::Trace_Weapon);

	AARCharacter* TargetChar = Cast<AARCharacter>(HitResult.GetActor());
	if (!TargetInfoWidget.IsValid() && TargetChar)
	{
		SAssignNew(TargetInfoWidget, SARTargetInfoWidget).OwnerHUD(this).IsEnabled(true).MyAttrComp(TargetChar->Attributes.Get())
			.HealthBarWidth(HUDTargetInfo.HealthBarWidth).HealthBarHeight(HUDTargetInfo.HealthBarHeight)
			.HealthBarPosX(Canvas->ClipX*HUDTargetInfo.HealthBarPosX)
			.HealthBarPosY(Canvas->ClipY*HUDTargetInfo.HealthBarPosY);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		///Pass our viewport a weak ptr to our widget
		if (GEngine->IsValidLowLevel())
		{
			GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(TargetInfoWidget.ToSharedRef()));
			/*Viewport's weak ptr will not give Viewport ownership of Widget*/
		}

		if (TargetInfoWidget.IsValid())
		{
			//////////////////////////////////////////////////////////////////////////////////////////////////
			///Set widget's properties as visible (sets child widget's properties recursively)
			TargetInfoWidget->SetVisibility(EVisibility::Visible);
		}
	}
	//else
	//{
	//	if (TargetInfoWidget.IsValid())
	//	{
	//		//TargetInfoWidget->SetVisibility(EVisibility::Collapsed);
	//		//TargetInfoWidget.Reset();
	//	}
	//}
	if (TargetInfoWidget.IsValid())
	{
		if (!TargetChar)
		{
			TargetInfoWidget->SetVisibility(EVisibility::Collapsed);
			TargetInfoWidget.Reset();
		}
	}

}


void AARHUD::DrawFloatingTextWidget()
{

}