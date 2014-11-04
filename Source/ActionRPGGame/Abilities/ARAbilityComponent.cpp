// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../Types/AREnumTypes.h"

#include "../ARPlayerController.h"
#include "../Abilities/ARAbility.h"
#include "Net/UnrealNetwork.h"

#include "ARAbilityComponent.h"

UARAbilityComponent::UARAbilityComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
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
		ActionBars.ActionBars[ActionBarIndex].ActionSlots[SlotID].Ability = GetWorld()->SpawnActor<AARAbility>(AbiltityDataTemp[AbilityIn.ActionIndex].AbilityClass);
		ActionBars.ActionBars[ActionBarIndex].ActionSlots[SlotID].Ability->OwningCharacter = OwningCharacter;
		ActionBars.ActionBars[ActionBarIndex].ActionSlots[SlotID].Ability->SetOwner(OwningCharacter);
		ActionBars.ActionBars[ActionBarIndex].ActionSlots[SlotID].Ability->Initialize();
		
		//it's only called on server.
		OnAbilityAdded.Broadcast(ActionBars.ActionBars[ActionBarIndex].ActionSlots[SlotID].Ability.Get());

		float abSize = sizeof(*ActionBars.ActionBars[ActionBarIndex].ActionSlots[SlotID].Ability.Get());
		GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, FString::FormatAsNumber(abSize));

		ClientOnAbilityAdded(SlotID, ActionBarIndex);
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
	OnActionAddedToBar.ExecuteIfBound(ActionBars.ActionBars[ActionBarIndex].ActionSlots[SlotID]);
}

void UARAbilityComponent::SetActiveAction(FActionSlotInfo ActionIn)
{
	ActiveAbility = ActionIn.Ability.Get(); //GetWorld()->SpawnActor<AARAbility>(AbiltityDataTemp[ActionIn.ActionIndex].AbilityClass);
//	ActiveAbility->OwningCharacter = OwningCharacter;
//	ActiveAbility->Initialize();
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

			IIARActionState* actionInterface = Cast<IIARActionState>(ActiveAction.Ability.Get());
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