// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "AREnumTypes.generated.h"

UENUM()
enum EAttrOp
{
	Attr_Add,
	Attr_Subtract,
	Attr_Multiply,
	Attr_Divide,
	Attr_Set
};

UENUM()
enum EARTraceType
{
	Trace_Weapon,
	Trace_UI,

	Trace_MAX
};

UENUM()
enum EARAbilitySlot
{
	Ability_Inventory,
	Ability_ActionBar,
	Ability_MAX
};

UENUM(BlueprintType)
enum EItemSlot
{
	Item_Head,
	Item_Shoulders,
	Item_Chest,
	Item_Hands,
	Item_Legs,
	Item_Foot,
	Item_TwoHanded, //takes both hands.
	Item_Weapon,  //either hand.
	Item_Inventory,
	Item_Blank, //special slot. This item exist only as data. It doesn't have object or mesh attached. Only texture.
	Item_MAX //keep it last;
};

UENUM(BlueprintType)
namespace EEquipmentSlot
{
	enum Type
	{
		Item_Head,
		Item_Shoulders,
		Item_Chest,
		Item_Hands,
		Item_Legs,
		Item_Foot,
		Item_LeftHandOne,
		Item_LeftHandActive,
		Item_RightHandOne,
		Item_RightHandActive,
		Item_Inventory,
		Item_MAX
	};
}