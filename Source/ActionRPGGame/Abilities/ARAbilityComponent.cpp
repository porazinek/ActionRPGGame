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

	bWantsInitializeComponent = true;

	for (int8 i = 0; i < 5; i++)
	{
		FAbilityInfo ab;
		ab.SlotID = i;
		ab.Ability = nullptr;
		ActionBarOne.Add(ab);
	}
}

void UARAbilityComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetOwnerRole() == ROLE_Authority)
	{
		ActionBars.ActionBars.Empty(ActionBars.ActionBars.Num());
		FActionSlotContainer asc;
		asc.ActionBarIndex = 0;
		for (int32 Idx = 0; Idx < 5; Idx++)
		{
			FActionSlotInfo asi;
			asi.SlotIndex = Idx;
			asi.ActionBarIndex = 0;
			asc.ActionSlots.Add(asi);
		}

		ActionBars.ActionBars.Add(asc);

		FActionSlotInfo abTest;
		abTest.ActionIndex = 0;

		AbilityBook.Add(abTest);
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
	DOREPLIFETIME_CONDITION(UARAbilityComponent, ActionBars, COND_OwnerOnly);
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

void UARAbilityComponent::AddAbilityToActionBar(FActionSlotInfo AbilityIn, int32 SlotID, int32 ActionBarIndex)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerAddAbilityToActionBar(AbilityIn, SlotID, ActionBarIndex);
	}
	else
	{

		ActionBars.ActionBars[ActionBarIndex].ActionSlots[SlotID].ActionIndex = AbilityIn.ActionIndex;
		ClientOnAbilityAdded(SlotID, ActionBarIndex);
		//if (ActionBarOne.Num() <= 5)
		//{
		//	//for (FAbilityInfo& ability : OwningController->AbilityInventory)
		//	//{
		//		//if (ability.AbilityName == AbilityIn.AbilityName)
		//		//{
		//			for (FAbilityInfo& abilityBar : ActionBarOne)
		//			{
		//				if (abilityBar.SlotID == SlotID)
		//				{
		//					if (abilityBar.Ability.IsValid())
		//					{
		//						abilityBar.Ability->Destroy();
		//						abilityBar.Ability.Reset();
		//					}

		//					if (!AbilityIn.AbilityType)
		//						return;

		//					FActorSpawnParameters SpawnInfo;
		//					SpawnInfo.bNoCollisionFail = true;

		//					AARAbility* tempAbi = GetWorld()->SpawnActor<AARAbility>(AbilityIn.AbilityType, SpawnInfo);
		//					tempAbi->SetOwner(GetOwner());
		//					
		//					abilityBar.AbilityName = AbilityIn.AbilityName;
		//					abilityBar.Ability = tempAbi;
		//				}
		//			}
		//		//}
		//	//}
		//}
	}
}
void UARAbilityComponent::ServerAddAbilityToActionBar_Implementation(FActionSlotInfo AbilityIn, int32 SlotID, int32 ActionBarIndex)
{
	AddAbilityToActionBar(AbilityIn, SlotID, ActionBarIndex);
}

bool UARAbilityComponent::ServerAddAbilityToActionBar_Validate(FActionSlotInfo AbilityIn, int32 SlotID, int32 ActionBarIndex)
{
	return true;
}

void UARAbilityComponent::ClientOnAbilityAdded_Implementation(int32 SlotID, int32 ActionBarIndex)
{
	OnActionAddedToBar.Broadcast(ActionBars.ActionBars[ActionBarIndex].ActionSlots[SlotID]);
}

void UARAbilityComponent::SetActiveAbility(int32 SlotID, int32 ActionBarIndex)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerSetActiveAbility(SlotID, ActionBarIndex);
	}
	else
	{
		ActiveAction = ActionBars.ActionBars[ActionBarIndex].ActionSlots[SlotID];
	}
}

void UARAbilityComponent::ServerSetActiveAbility_Implementation(int32 SlotID, int32 ActionBarIndex)
{
	SetActiveAbility(SlotID, ActionBarIndex);
}
bool UARAbilityComponent::ServerSetActiveAbility_Validate(int32 SlotID, int32 ActionBarIndex)
{
	return true;
}


void UARAbilityComponent::FireAbility()
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerFireAbility();
	}
	else
	{
		if (ActiveAction.Ability.IsValid())
		{//	ActiveAction.Ability->StartAction();

			IIARActionState* actionInterface = InterfaceCast<IIARActionState>(ActiveAction.Ability.Get());
			if (actionInterface)
			{
				actionInterface->InputPressed();
			}
		}
	}
}
void UARAbilityComponent::ServerFireAbility_Implementation()
{
	FireAbility();
}
bool UARAbilityComponent::ServerFireAbility_Validate()
{
	return true;
}