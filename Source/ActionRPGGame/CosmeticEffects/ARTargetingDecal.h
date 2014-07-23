// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "ARTargetingDecal.generated.h"

UCLASS(hidecategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming), editinlinenew, meta = (BlueprintSpawnableComponent))
class UARTargetingDecal : public UActorComponent
{
	GENERATED_UCLASS_BODY()
public:

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction);
	virtual void DestroyComponent();
	UPROPERTY(EditAnywhere, Category = "Decal")
		UMaterialInterface* DecalMaterial;

	UPROPERTY()
		UDecalComponent* DecalComponent;

	UPROPERTY(EditAnywhere, Category = "Decal")
		float UpdateInterval;
	UPROPERTY(EditAnywhere, Category = "Decal")
		FVector DecalSize;

	UPROPERTY(BlueprintReadWrite, Category = "Owner")
		APawn* OwningCharacter;
	UFUNCTION(BlueprintCallable, Category = "Targeting Component")
		void Initialize(APawn* OwningCharacterIn);

	UFUNCTION()
		void UpdateDecalComponentPosition();

private:
	float CurrentUpdateInterval;
};



