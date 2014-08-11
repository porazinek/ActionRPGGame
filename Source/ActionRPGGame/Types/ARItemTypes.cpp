// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "ActionRPGGame.h"

#include "ARItemTypes.h"


void FARItemPickerContainer::RemoveItemContainer(FARItemPickupCont* ToRemove)
{
	if (ListOfItems.Num() > 0)
	{
		for (int32 i = 0; i < ListOfItems.Num(); i++)
		{
			if (ListOfItems[i].ItemData == ToRemove->ItemData)
			{
				ListOfItems.RemoveAt(i);
				return;
			}
		}
	}
}