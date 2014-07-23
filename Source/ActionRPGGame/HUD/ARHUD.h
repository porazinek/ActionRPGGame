// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "ARHUD.generated.h"

const float DAMAGE_FADE_DURATION = 1.0f;

USTRUCT(BlueprintType)
struct FARHUDTargetInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "HUD")
	float HealthBarWidth;
	UPROPERTY(EditAnywhere, Category = "HUD")
	float HealthBarHeight;
	UPROPERTY(EditAnywhere, Category = "HUD")
	float HealthBarPosX;
	UPROPERTY(EditAnywhere, Category = "HUD")
	float HealthBarPosY;
	UPROPERTY(EditAnywhere, Category = "HUD")
		FVector2D HealthPosition;
	UPROPERTY(EditAnywhere, Category = "HUD")
		FVector2D HealthSize;
};
USTRUCT()
struct FDamageHudIndicator
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY()
		float RotationAngle;

	UPROPERTY()
		float DamageAmount;

	UPROPERTY()
		float FadeTime;

	FDamageHudIndicator()
		: RotationAngle(0.0f)
		, DamageAmount(0.0f)
		, FadeTime(0.0f)
	{
	}
};
UCLASS()
class AARHUD : public AHUD
{
	GENERATED_UCLASS_BODY()

public:
	virtual void DrawHUD() override;

	TSharedPtr<class SARHUDWidget> HUDWidget;

	TSharedPtr<class SARTargetInfoWidget> TargetInfoWidget;

	//TSharedPtr<class SARFloatingCombatTextWidget> FloatingCombatTextWidget;

	virtual void BeginPlay() override;

	void DrawDamageIndicators();
	void PawnDamaged(FVector HitLocation, float DamageAmount, TSubclassOf<UDamageType> DamageClass);
	UPROPERTY()
		TArray<struct FDamageHudIndicator> DamageIndicators;

	UPROPERTY()
		TWeakObjectPtr<class AARCharacter> OwnerChar;

	UPROPERTY(BlueprintReadWrite, Category = "HUD|Inventory")
		bool DrawInventoryWidget;
	UPROPERTY(EditAnywhere, Category = "Target Info")
		FARHUDTargetInfo HUDTargetInfo;
protected:
	UPROPERTY(EditAnywhere, Category = "UI|Castbar")
		float CastbarHeight;
	UPROPERTY(EditAnywhere, Category = "UI|Castbar")
		float CastbarWidth;
	/*
		0 - center.
		0 < Push 
	*/
	UPROPERTY(EditAnywhere, Category = "UI|Castbar")
		float CastbarPositionX;
	/*
		0 - center.
		0 < Push bottom.
		0 > Push Top.
	*/
	UPROPERTY(EditAnywhere, Category = "UI|Castbar")
		float CastbarPositionY;

	UPROPERTY(EditAnywhere, Category = "UI")
		float SCTLifeTime;
	float CurrentSCTLifeTime;

	/*
		Draws slate widget Health/Stamina/Energy for current Target.
		I probably wouldn't want this in my game, but it will be here for reference ;).
	*/
	void DrawResourceWidgetForTarget();
	UFUNCTION()
	void DrawResourceBars();

	void DrawFloatingTextWidget();

	FVector2D GetFCTPosition() const;

	FVector2D FCTPosition;

	EVisibility GetFCTVisibility() const;

private:
	UTexture2D* CrosshairTex;
};



