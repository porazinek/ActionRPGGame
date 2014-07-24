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
#include "../Componenets/ARAttributeBaseComponent.h"

#include "../Abilities/ARAbility.h"

#include "../Types/ARStructTypes.h"

#include "../BlueprintLibrary/ARTraceStatics.h"

#include "ARHUD.h"

AARHUD::AARHUD(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("Texture2D'/Game/SharedAssets/HUD/crosshair.crosshair'"));
	CrosshairTex = CrosshairTexObj.Object;

	DrawInventoryWidget = false;

	FCTAnimDirection = FVector2D(1, 1);
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

	if (GetOwningPawn() && !OwnerChar.IsValid())
	{
		OwnerChar = Cast<AARCharacter>(GetOwningPawn());
	}

	if (GetOwningPlayerController() && !OwnerPC.IsValid())
	{
		OwnerPC = Cast<AARPlayerController>(GetOwningPlayerController());
	}
	/*
		Don't draw if we don't have proper character.
		WE REALLY NEED PROPER CHARACTER!!!!
	*/
	if (!HUDWidget.IsValid() && OwnerChar.IsValid())
	{
		AARPlayerController* MyPC = Cast<AARPlayerController>(GetOwningPlayerController());
		//null on BeginPlay

		//TWeakObjectPtr<UARAttributeComponent> MyAttr = Character->Attributes.Get();
		SAssignNew(HUDWidget, SARHUDWidget).OwnerHUD(this).IsEnabled(true).MyPC(MyPC).MyChar(OwnerChar).MyAttrComp(OwnerChar->Attributes.Get()).CastbarHeight(CastbarHeight)
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
			HUDWidget->TargetAttrComp.BindUObject(this, &AARHUD::GetTargetAttributes);
		}
	}
	DrawOwnerResources();
	DrawTargetHealth();
	//DrawResourceWidgetForTarget();
	DrawDamageIndicators();
	DrawCastingBar();
	if (OwnerChar.IsValid())
	{
		if (!OwnerChar->Attributes->OnInstigatorDamage.IsBoundToObject(this))
		{
			OwnerChar->Attributes->OnInstigatorDamage.AddUObject(this, &AARHUD::PawnDamaged);
		}
		
		if (Canvas)
		{
			//FVector temp = Canvas->Project(OwnerChar->Attributes->ChangedAttribute.DamageTarget->GetActorLocation());
			FVector temp = Canvas->Project(OwnerChar->Attributes->UIDamage.Location);
		}
	}
}

void AARHUD::PawnDamaged(FARUIDamage UIDamage)
{
	// Calculate the rotation 	
	//AUTCharacter* UTC = UTPlayerOwner->GetUTCharacter();
	if (OwnerChar.IsValid())	// If have a pawn and it's alive...
	{
		FVector CharacterLocation;
		FRotator CharacterRotation;

		OwnerChar->GetActorEyesViewPoint(CharacterLocation, CharacterRotation);
		FVector HitSafeNormal = (UIDamage.Location - CharacterLocation).SafeNormal2D();
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
				DamageIndicators[i].CurrentLocation = FVector2D::ZeroVector;
				DamageIndicators[i].AnimDirection = FVector2D::ZeroVector;
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

		DamageIndicators[BestIndex].FadeTime = FCTSettings.FCTLifeTime;
		DamageIndicators[BestIndex].RotationAngle = FinalAng;
		DamageIndicators[BestIndex].DamageAmount = UIDamage.Value;
		DamageIndicators[BestIndex].Location = UIDamage.Location;
		DamageIndicators[BestIndex].AnimDirection.X = FMath::FRandRange(-FCTAnimDirection.X, FCTAnimDirection.X);
		DamageIndicators[BestIndex].AnimDirection.Y = FMath::FRandRange(-FCTAnimDirection.Y, FCTAnimDirection.Y);
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
			DrawColor.A = 1.0 * (DamageIndicators[i].FadeTime / FCTSettings.FCTLifeTime);

			float Size = 384 * (Canvas->ClipY / 720.0f);
			float Half = Size * 0.5;
			FVector vecTemp = Canvas->Project(DamageIndicators[i].Location);
			FVector2D vec2d;
			DamageIndicators[i].CurrentLocation.X += DamageIndicators[i].AnimDirection.X;
			DamageIndicators[i].CurrentLocation.Y += DamageIndicators[i].AnimDirection.Y;
			vec2d.X += FMath::FRandRange(-FCTSettings.RandomOffset, FCTSettings.RandomOffset);
			vec2d.Y += FMath::FRandRange(-FCTSettings.RandomOffset, FCTSettings.RandomOffset);
			vec2d.X = vecTemp.X + DamageIndicators[i].CurrentLocation.X;
			vec2d.Y = vecTemp.Y + DamageIndicators[i].CurrentLocation.Y;
			//FVector2D((Canvas->ClipX * 0.5) - Half, (Canvas->ClipY * 0.5) - Half)
			FCanvasTextItem TextItem = FCanvasTextItem(vec2d, FText::AsNumber(DamageIndicators[i].DamageAmount), FCTSettings.FontType, FCTSettings.FontColor);
			
			TextItem.Scale = FVector2D(FCTSettings.FontScale, FCTSettings.FontScale);

			Canvas->DrawItem(TextItem);

			DamageIndicators[i].FadeTime -= RenderDelta;
		}
	}
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
void AARHUD::DrawResourceBar(float CurrentValue, float MaxValue, FVector2D Size, float PosX, float PosY, FVector2D Offset, FLinearColor Foreground, FLinearColor Background)
{
	float HealthBarPrecentage = (CurrentValue / MaxValue);
	FCanvasTileItem TileItem = FCanvasTileItem(FVector2D((Canvas->ClipX*PosX) + Offset.X, (Canvas->ClipY*PosY) + Offset.Y),
		Size, Background);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);
	
	TileItem.Size = FVector2D(200 * HealthBarPrecentage, 20);
	TileItem.SetColor(Foreground);

	Canvas->DrawItem(TileItem);
}

void AARHUD::DrawTargetHealth()
{
	FHitResult HitResult = UARTraceStatics::GetHitResult(10000, NAME_None, GetOwningPawn(), false, false, EARTraceType::Trace_Weapon);
	AARCharacter* TargetChar = Cast<AARCharacter>(HitResult.GetActor());

	if (TargetChar)
	{
		DrawResourceBar(TargetChar->Attributes->Health, TargetChar->Attributes->MaxHealth, TargetInfo.HealthSize, 
			TargetInfo.Position.X, TargetInfo.Position.Y, FVector2D(0, 0), TargetInfo.HealthColor,
			ResourceBars.BackgroundColor);
	}
}

void AARHUD::DrawOwnerResources()
{
	if (!OwnerChar.IsValid())
		return;

	//Draw Health
	DrawResourceBar(OwnerChar->Attributes->Health, OwnerChar->Attributes->MaxHealth, ResourceBars.HealthSize,
		ResourceBars.Position.X, ResourceBars.Position.Y, FVector2D(0, 0), 
		ResourceBars.HealthColor, ResourceBars.BackgroundColor);

	DrawResourceBar(OwnerChar->Attributes->Energy, OwnerChar->Attributes->MaxEnergy, ResourceBars.EnergySize,
		ResourceBars.Position.X, ResourceBars.Position.Y, FVector2D(0, ResourceBars.EnergySize.Y),
		ResourceBars.EnergyColor, ResourceBars.BackgroundColor);

	DrawResourceBar(OwnerChar->Attributes->Stamina, OwnerChar->Attributes->MaxStamina, ResourceBars.StaminaSize,
		ResourceBars.Position.X, ResourceBars.Position.Y, FVector2D(0, ResourceBars.StaminaSize.Y + ResourceBars.EnergySize.Y),
		ResourceBars.StaminaColor, ResourceBars.BackgroundColor);
}

void AARHUD::DrawCastingBar()
{
	if (!OwnerPC.IsValid())
		return;

	if (OwnerPC->ActiveAbility)
	{
		DrawResourceBar(OwnerPC->ActiveAbility->CurrentCastTime, OwnerPC->ActiveAbility->MaxCastTime,
			CastingBar.Size, CastingBar.Position.X, CastingBar.Position.Y, FVector2D(0, 0),
			CastingBar.Color, CastingBar.BackgroundColor
			);
	}
}

TWeakObjectPtr<class UARAttributeComponent> AARHUD::GetTargetAttributes() const
{
	FHitResult HitResult = UARTraceStatics::GetHitResult(10000, NAME_None, GetOwningPawn(), false, false, EARTraceType::Trace_Weapon);
	AARCharacter* TargetChar = Cast<AARCharacter>(HitResult.GetActor());

	if (TargetChar)
	{
		return TargetChar->Attributes.Get();
	}
	return nullptr;
}
