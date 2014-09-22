// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"
#include "../Types/ARStructTypes.h"

#include "Net/UnrealNetwork.h"

#include "ARItem.h"

AARItem::AARItem(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	bReplicates = true;
	bNetUseOwnerRelevancy = true;
}

void AARItem::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARItem, ARItemID);
	DOREPLIFETIME(AARItem, ItemIndex);
}