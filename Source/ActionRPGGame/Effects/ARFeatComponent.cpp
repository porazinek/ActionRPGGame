// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "ARFeat.h"

#include "../Componenets/ARAttributeComponent.h"

#include "ARFeatComponent.h"

UARFeatComponent::UARFeatComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bWantsInitializeComponent = true;
}

void UARFeatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	Attributes = GetOwner()->FindComponentByClass<UARAttributeComponent>();
	if (FeatClasses.Num() > 0)
	{
		for (TSubclassOf<UARFeat> Feat : FeatClasses)
		{
			if (!Feat)
				return;
			UARFeat* tempFeat = ConstructObject<UARFeat>(Feat, this);
			tempFeat->Attributes = Attributes;
			tempFeat->EffectTarget = GetOwner();
			tempFeat->EffectInstigator = GetOwner();
			tempFeat->InitializeEffect();
			ActiveFeats.Add(tempFeat);
		}
	}
}