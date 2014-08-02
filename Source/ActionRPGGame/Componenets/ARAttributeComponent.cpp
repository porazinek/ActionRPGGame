// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../Types/AREnumTypes.h"

#include "Net/UnrealNetwork.h"

#include "../Effects/AREffectType.h"
#include "../Effects/DefaultEffects/ARAbilityCostEffect.h"

#include "ARAttributeComponent.h"

UARAttributeComponent::UARAttributeComponent(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = true;
	bAutoRegister = true;

	bWantsInitializeComponent = true;
}

void UARAttributeComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UARAttributeComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetOwnerRole() < ROLE_Authority)
		return;
	MaxHealth = Health;
	MaxEnergy = Energy;
	MaxStamina = Stamina;

	DefaultEffects.Add(ConstructObject<UARAbilityCostEffect>(UARAbilityCostEffect::StaticClass()));
}

void UARAttributeComponent::Initialize()
{
	Super::Initialize();
	if (GetOwnerRole() < ROLE_Authority)
		return;
	for (UAREffectType* effect : DefaultEffects)
	{
		effect->EffectTarget = PlayerController;
		effect->EffectInstigator = PlayerCharacter;
		effect->EffectCausedBy = PlayerCharacter;
		effect->Initialize();
	}
}

void UARAttributeComponent::OnRegister()
{
	Super::OnRegister();
	if (GetOwnerRole() < ROLE_Authority)
		return;
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


