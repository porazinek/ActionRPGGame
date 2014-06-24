// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "ARAttributeBaseComponent.h"
#include "../Types/ARStructTypes.h"


#include "ARAttributeComponent.generated.h"

UCLASS(hidecategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming), editinlinenew, meta = (BlueprintSpawnableComponent))
class UARAttributeComponent : public UARAttributeBaseComponent
{
	GENERATED_UCLASS_BODY()

		virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_Health, Category = "Derived Attribute")
		float Health;
	UFUNCTION()
		void OnRep_Health();

};



