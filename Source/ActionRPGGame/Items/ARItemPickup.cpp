// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "ActionRPGGame.h"
#include "../Types/ARStructTypes.h"
#include "../ARPlayerController.h"

#include "ARItemDataAsset.h"

#include "Net/UnrealNetwork.h"

#include "ARItemPickup.h"

AARItemPickup::AARItemPickup(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	bReplicates = true;
	bNetUseOwnerRelevancy = true;
	
}
void AARItemPickup::BeginPlay()
{
	Super::BeginPlay();
}

void AARItemPickup::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	ItemContainer.ItemData = ItemsData;

	MarkPackageDirty();
}

void AARItemPickup::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARItemPickup, ItemPickups);
}

void AARItemPickup::GiveAllItemsTo(class AARPlayerController* PickupInstigator)
{
	if (!PickupInstigator) //really shouldn't be ever null.
		return;
}

