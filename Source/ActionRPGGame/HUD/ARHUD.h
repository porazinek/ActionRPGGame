// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "../Types/ARStructTypes.h"

#include "ARHUD.generated.h"
/*
	For the record.
	I want to use Slate for entire UI.
	But I dunno how UMG will handle my Slate Widgets if at all.
	So for now I will have some duplicated functionality in Canvas and Slate, and some exclusive Slate
	Functionality. Like Inventory, because it's so much easier to do interactive UI in Slate.
	In any case I won't push slate UI much, until there is more info about Slate C++ Widgets, UMG Interaction.
*/
const float DAMAGE_FADE_DURATION = 3.0f;

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
public:
	UPROPERTY()
		float RotationAngle;

	UPROPERTY()
		float DamageAmount;

	UPROPERTY()
		float FadeTime;

	UPROPERTY()
		FVector Location;

	UPROPERTY()
		FVector2D CurrentLocation;

	UPROPERTY()
		FVector2D AnimDirection;

	FDamageHudIndicator()
		: RotationAngle(0.0f)
		, DamageAmount(0.0f)
		, FadeTime(0.0f)
		, Location(FVector::ZeroVector)
	{
	}
};

USTRUCT(BlueprintType)
struct FFCTDisplaySettings
{
	GENERATED_USTRUCT_BODY();
public:
	UPROPERTY(EditAnywhere, Category = "FCT")
		float FontScale;
	UPROPERTY(EditAnywhere, Category = "FCT")
		FLinearColor FontColor;
	UPROPERTY(EditAnywhere, Category = "FCT")
		UFont* FontType;

	UPROPERTY(EditAnywhere, Category = "FCT")
		float FCTLifeTime;

	UPROPERTY(EditAnywhere, Category = "FCT")
		float RandomOffset;
};

USTRUCT(BlueprintType)
struct FOwnerResourceBars
{
	GENERATED_USTRUCT_BODY();
public:
	UPROPERTY(EditAnywhere, Category = "Bars")
		FVector2D Position;

	UPROPERTY(EditAnywhere, Category = "Bars")
		FLinearColor HealthColor;
	UPROPERTY(EditAnywhere, Category = "Bars")
		FVector2D HealthSize;

	UPROPERTY(EditAnywhere, Category = "Bars")
		FLinearColor EnergyColor;
	UPROPERTY(EditAnywhere, Category = "Bars")
		FVector2D EnergySize;

	UPROPERTY(EditAnywhere, Category = "Bars")
		FLinearColor StaminaColor;
	UPROPERTY(EditAnywhere, Category = "Bars")
		FVector2D StaminaSize;

	UPROPERTY(EditAnywhere, Category = "Bars")
		FLinearColor BackgroundColor;
};

USTRUCT(BlueprintType)
struct FUITargetInfo
{
	GENERATED_USTRUCT_BODY();
public:
	UPROPERTY(EditAnywhere, Category = "Bars")
		FVector2D Position;

	UPROPERTY(EditAnywhere, Category = "Bars")
		FLinearColor HealthColor;
	UPROPERTY(EditAnywhere, Category = "Bars")
		FVector2D HealthSize;

	UPROPERTY(EditAnywhere, Category = "Bars")
		FLinearColor EnergyColor;
	UPROPERTY(EditAnywhere, Category = "Bars")
		FVector2D EnergySize;

	UPROPERTY(EditAnywhere, Category = "Bars")
		FLinearColor StaminaColor;
	UPROPERTY(EditAnywhere, Category = "Bars")
		FVector2D StaminaSize;

	UPROPERTY(EditAnywhere, Category = "Bars")
		FLinearColor BackgroundColor;
};

USTRUCT(BlueprintType)
struct FUICastingBar
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Bars")
		FVector2D Position;

	UPROPERTY(EditAnywhere, Category = "Bars")
		FLinearColor Color;
	UPROPERTY(EditAnywhere, Category = "Bars")
		FVector2D Size;

	UPROPERTY(EditAnywhere, Category = "Bars")
		FLinearColor BackgroundColor;
};

UCLASS()
class AARHUD : public AHUD
{
	GENERATED_UCLASS_BODY()

public:
	virtual void DrawHUD() override;
	virtual void PostRender() override;

	UPROPERTY(EditAnywhere, Category = "Mouse Look")
		float LookSpeed;

	bool TargetModeSwaped;
	int8 TargetModeDirtyFrames;
	FVector DeprojectWorldPosition;

	FVector DeprojectWorldDirection;

	TSharedPtr<class SARHUDWidget> HUDWidget;

	TSharedPtr<class SARTargetInfoWidget> TargetInfoWidget;

	//TSharedPtr<class SARFloatingCombatTextWidget> FloatingCombatTextWidget;

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "UI Settings")
		FFCTDisplaySettings FCTSettings;

	UPROPERTY(EditAnywhere, Category = "UI Settings")
		FOwnerResourceBars ResourceBars;

	UPROPERTY(EditAnywhere, Category = "UI Settings")
		FUITargetInfo TargetInfo;

	UPROPERTY(EditAnywhere, Category = "UI Settings")
		FUICastingBar CastingBar;
	void DrawDamageIndicators();
	void PawnDamaged(FARUIDamage UIDamage);

	UPROPERTY(EditAnywhere, Category = "FCT")
	FVector2D FCTAnimDirection;

	UPROPERTY()
		TArray<struct FDamageHudIndicator> DamageIndicators;

	UPROPERTY()
		TWeakObjectPtr<class AARCharacter> OwnerChar;

	UPROPERTY()
		TWeakObjectPtr<class AARPlayerController> OwnerPC;

	UPROPERTY(BlueprintReadWrite, Category = "HUD|Inventory")
		bool DrawInventoryWidget;
	UPROPERTY(EditAnywhere, Category = "Target Info")
		FARHUDTargetInfo HUDTargetInfo;
protected:

	bool CursorDirty;
	int32 DirtyFrameCount;

	UPROPERTY()
		ULocalPlayer* HUDLocalPlayer;

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

	TWeakObjectPtr<class UARAttributeComponent> GetTargetAttributes() const;

	/*
		Draws slate widget Health/Stamina/Energy for current Target.
		I probably wouldn't want this in my game, but it will be here for reference ;).
	*/
	void DrawResourceWidgetForTarget();

	

	UFUNCTION()
		void DrawResourceBar(float CurrentValue, float MaxValue, FVector2D Size, float PosX, float PosY, FVector2D Offset, FLinearColor Foreground, FLinearColor Background);

	void DrawTargetHealth();

	/*
		Draw Health, Energy and Stamina bars.
	*/
	void DrawOwnerResources();

	void DrawCastingBar();

	//void DrawFloatingTextWidget();

private:
	UTexture2D* CrosshairTex;
};



