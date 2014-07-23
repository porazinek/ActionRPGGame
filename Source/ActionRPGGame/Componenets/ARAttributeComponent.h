// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "ARAttributeBaseComponent.h"
#include "../Types/ARStructTypes.h"


#include "ARAttributeComponent.generated.h"

UCLASS(hidecategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming), editinlinenew, meta = (BlueprintSpawnableComponent))
class UARAttributeComponent : public UARAttributeBaseComponent
{
	GENERATED_UCLASS_BODY()
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	virtual void OnRegister() override;

	virtual void InitializeComponent() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_Health, Category = "Derived Attribute")
		float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Health, Category = "Derived Attribute")
		float Energy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Health, Category = "Derived Attribute")
		float Stamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Derived Attribute")
		float MaxHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Derived Attribute")
		float MaxEnergy;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Derived Attribute")
		float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Derived Attribute")
		float Armor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Derived Attribute")
		float AttackPower;

	UFUNCTION()
		void OnRep_Health();

};



