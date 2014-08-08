// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#ifndef __ACTIONRPGGAME_H__
#define __ACTIONRPGGAME_H__



#include "Engine.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ParticleDefinitions.h"
#include "GameplayTags.h"

#include "../JsonDataAsset/JsonDataAsset.h"
#include "../Items/ARItemsData.h"

DECLARE_LOG_CATEGORY_EXTERN(AR, Log, All);

#define COLLISION_WEAPON_CHANNEL ECC_GameTraceChannel1
#define COLLISION_PROJECTILE_CHANNEL ECC_GameTraceChannel2
#define COLLISION_GUI_CHANNEL ECC_GameTraceChannel3

/*
	Custom Serialization overrides for FArchive.
*/
inline FArchive& operator<<(FArchive &Ar, FDataTestStruct& Data)
{
	Ar << Data.TestString;
};

inline FArchive& operator<<(FArchive &Ar, FSlateColor& Data)
{
	Ar << Data.SpecifiedColor;
	Ar << Data.ColorUseRule;
	return Ar;
};

inline FArchive& operator<<(FArchive &Ar, FMargin& Data)
{
	Ar << Data.Left;
	Ar << Data.Top;
	Ar << Data.Right;
	Ar << Data.Bottom;
	return Ar;
};

inline FArchive& operator<<(FArchive &Ar, FSlateBrush& Data)
{
	Ar << Data.ImageSize;
	Ar << Data.DrawAs;
	Ar << Data.Margin;
	Ar << Data.TintColor;
	Ar << Data.Tiling;
	Ar << Data.Mirroring;
	Ar << Data.ImageType;
	return Ar;
};

inline FArchive& operator<<(FArchive &Ar, FAttribute& Data)
{
	Ar << Data.AttributeName;
	Ar << Data.ModValue;
	return Ar;
};

inline FArchive& operator<<(FArchive &Ar, FItemAttribute& Data)
{
	Ar << Data.AttributeName;
	Ar << Data.ModValue;
	Ar << Data.IsStackable;
	return Ar;
};

inline FArchive& operator<<(FArchive &Ar, FItemData& Data)
{
	Ar << Data.Durability;
	Ar << Data.Weight;
	Ar << Data.Attributes;
	Ar << Data.ItemEffects;
	Ar << Data.Mesh;
	return Ar;
};

inline FArchive& operator<<(FArchive &Ar, FARItemInfo& Data)
{
	Ar << Data.ItemName;
	Ar << Data.ItemDescription;
	//Ar << Data.ItemIcon;
	Ar << Data.ItemInfo;
	Ar << Data.ItemSlot;
	return Ar;
};

#endif

