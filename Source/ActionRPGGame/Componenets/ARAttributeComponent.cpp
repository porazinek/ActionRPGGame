// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../Types/AREnumTypes.h"

#include "Net/UnrealNetwork.h"

#include "ARAttributeComponent.h"

UARAttributeComponent::UARAttributeComponent(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = true;

	bWantsInitializeComponent = true;
}

void UARAttributeComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UARAttributeComponent::InitializeComponent()
{
	Super::InitializeComponent();
	MaxHealth = Health;
	MaxEnergy = Energy;
	MaxStamina = Stamina;
}

void UARAttributeComponent::OnRegister()
{
	Super::OnRegister();
}

void UARAttributeComponent::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARAttributeComponent, Health);
	DOREPLIFETIME(UARAttributeComponent, Energy);
	DOREPLIFETIME(UARAttributeComponent, Stamina);
	DOREPLIFETIME(UARAttributeComponent, MaxHealth);
	DOREPLIFETIME(UARAttributeComponent, MaxEnergy);
	DOREPLIFETIME(UARAttributeComponent, MaxStamina);
	DOREPLIFETIME(UARAttributeComponent, Armor);
	DOREPLIFETIME(UARAttributeComponent, AttackPower);
}

void UARAttributeComponent::OnRep_Health()
{
	GetAttributeModified();
	//SetAttributeModified(20, "Health");
}


