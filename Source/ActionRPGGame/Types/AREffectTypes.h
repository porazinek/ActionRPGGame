// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "AREnumTypes.h"
#include "Slate.h"
#include "AREffectTypes.generated.h"

USTRUCT(BlueprintType)
struct FEffectHandle
{
	GENERATED_USTRUCT_BODY()
public:

	inline void SetHandle(int32 HandleIn)
	{
		Handle = HandleIn;
	};

	inline bool operator==(const FEffectHandle& Other) const
	{
		Handle == Other.Handle;
	};
	inline bool operator!=(const FEffectHandle& Other) const
	{
		Handle != Other.Handle;
	};

	FAttribute() {};
private:
	int32 Handle;
};

USTRUCT()
struct FEffectSpec
{
	GENERATED_USTRUCT_BODY()
public:
	FEffectHandle Handle;
};
