// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "Widgets/ARButtonWidget.h"
#include "Widgets/ARHUDWidget.h"

#include "../ARCharacter.h"
#include "../ARPlayerController.h"
#include "../Componenets/AREquipmentComponent.h"

#include "ARHUD.h"

AARHUD::AARHUD(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("Texture2D'/Game/SharedAssets/HUD/crosshair.crosshair'"));
	CrosshairTex = CrosshairTexObj.Object;

	DrawInventoryWidget = false;
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
}

void AARHUD::BeginPlay()
{
	//AARCharacter* MyChar = Cast<AARCharacter>(GetOwningPawn());
	AARPlayerController* MyPC = Cast<AARPlayerController>(GetOwningPlayerController());
	//null on BeginPlay
	AARCharacter* Character = NULL;
	if (MyPC)
	{
		Character = Cast<AARCharacter>(MyPC->GetPawn());
	}
	SAssignNew(HUDWidget, SARHUDWidget).OwnerHUD(this).IsEnabled(true).MyPC(MyPC);
	
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
	}
}