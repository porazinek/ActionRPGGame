// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../Types/AREnumTypes.h"

#include "../ARPlayerController.h"
#include "../Abilities/ARAbility.h"
#include "Net/UnrealNetwork.h"

#include "ARAbilityComponent.h"

UARAbilityComponent::UARAbilityComponent(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = true;
	UpdateActionBarOne = false;
	for (int8 i = 0; i < 5; i++)
	{
		FAbilityInfo ab;
		ab.SlotID = i;
		ab.Ability = nullptr;
		ActionBarOne.Add(ab);
	}
}

void UARAbilityComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UARAbilityComponent::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARAbilityComponent, ActionBarOne);
}

void UARAbilityComponent::OnRep_ActionBarOne()
{
	UpdateActionBarOne = true;
}

void UARAbilityComponent::AddAbility(FAbilityInfo AbilityIn)
{
	if (!OwningController)
		return;

	OwningController->AddAbilityToInventory(AbilityIn);
}

void UARAbilityComponent::AddAbilityToActionBar(FAbilityInfo AbilityIn, int32 SlotID)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerAddAbilityToActionBar(AbilityIn, SlotID);
	}
	else
	{
		if (ActionBarOne.Num() <= 5)
		{
			//for (FAbilityInfo& ability : OwningController->AbilityInventory)
			//{
				//if (ability.AbilityName == AbilityIn.AbilityName)
				//{
					for (FAbilityInfo& abilityBar : ActionBarOne)
					{
						if (abilityBar.SlotID == SlotID)
						{
							if (abilityBar.Ability.IsValid())
							{
								abilityBar.Ability->Destroy();
								abilityBar.Ability.Reset();
							}

							if (!AbilityIn.AbilityType)
								return;

							FActorSpawnParameters SpawnInfo;
							SpawnInfo.bNoCollisionFail = true;

							AARAbility* tempAbi = GetWorld()->SpawnActor<AARAbility>(AbilityIn.AbilityType, SpawnInfo);
							tempAbi->SetOwner(GetOwner());
							
							abilityBar.AbilityName = AbilityIn.AbilityName;
							abilityBar.Ability = tempAbi;
						}
					}
				//}
			//}
		}
	}
}
void UARAbilityComponent::ServerAddAbilityToActionBar_Implementation(FAbilityInfo AbilityIn, int32 SlotID)
{
	AddAbilityToActionBar(AbilityIn, SlotID);
}

bool UARAbilityComponent::ServerAddAbilityToActionBar_Validate(FAbilityInfo AbilityIn, int32 SlotID)
{
	return true;
}