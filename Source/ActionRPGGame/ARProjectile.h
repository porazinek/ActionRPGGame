// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "ARProjectile.generated.h"

UCLASS()
class AARProjectile : public AActor
{
	GENERATED_UCLASS_BODY()
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		TSubobjectPtr<class UProjectileMovementComponent> Movement;
};



