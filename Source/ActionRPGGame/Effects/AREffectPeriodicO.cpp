// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"
#include "../Componenets/ARAttributeBaseComponent.h"

#include "../ActionState/ARActionStateComponent.h"

#include "Net/UnrealNetwork.h"
#include "AREffectPeriodicO.h"

UAREffectPeriodicO::UAREffectPeriodicO(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ReplicationTest = true;

}
void UAREffectPeriodicO::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAREffectPeriodicO, ReplicationTest);
}
void UAREffectPeriodicO::OnRep_Test()
{
	float test = 10;
}
void UAREffectPeriodicO::TickMe(float DeltaTime)
{
	CurrentDuration += DeltaTime;
	ReplicationTest = false;
	if (CurrentDuration >= MaxDuration)
	{
		CurrentDuration = 0;
		ReplicationTest = true;
		OnRemoved();
	}
}

void UAREffectPeriodicO::OnRemoved()
{
	if (!EffectTarget.IsValid())
		return;

	UARAttributeBaseComponent* attr = EffectTarget->FindComponentByClass<UARAttributeBaseComponent>();

	attr->RemovedPeriodicEffect_TEMP(this);
}