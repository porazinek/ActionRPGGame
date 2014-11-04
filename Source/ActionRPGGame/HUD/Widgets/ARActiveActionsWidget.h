// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../ARHUD.h"

#include "../../Types/ARStructTypes.h"

#include "SlateBasics.h"

class SARActiveActionsWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SARActiveActionsWidget)
	{}
	/*See private declaration of OwnerHUD below.*/
		SLATE_ARGUMENT(TWeakObjectPtr<class AARHUD>, OwnerHUD)

		SLATE_ARGUMENT(TWeakObjectPtr<class AARPlayerController>, MyPC)
	SLATE_END_ARGS()

public:
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/////Needed for every widget
	/////Builds this widget and any of it's children
	void Construct(const FArguments& InArgs);

private:
	TWeakObjectPtr<class AARHUD> OwnerHUD;

	TWeakObjectPtr<class AARPlayerController> MyPC;
};



