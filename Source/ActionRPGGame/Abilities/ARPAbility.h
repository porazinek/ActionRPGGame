// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "../ActionState/IARActionState.h"

#include "ARPAbility.generated.h"

UCLASS(hidecategories = (Input, Movement, Collision, Rendering, "Utilities|Transformation"), MinimalAPI, Blueprintable, notplaceable)
class AARPAbility : public AActor, public IIARActionState
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(Replicated)
	bool BlankRep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Casting Speed")
		float CastingSpeed;

	virtual void Tick(float DeltaSeconds) OVERRIDE;

	/* [client] OVERIDE from IIARActionState */
	virtual void InputPressed() OVERRIDE;

	/*[Server]*/
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerStartAction();
	/*[Authorative]*/
	virtual void StartAction();

	/*
		These are client prediction helpers for drawing UI.
		They are purerly cosmetic, and do not affect gameplay,
		but they help with increasing smoothnes of gameplay,
		as player doesn't need to wait for notification from server about 
		various states of ability.
		Besides that, real state of ability is not directly accessible on client anyway.
		It's purerly server-side.
	*/
protected:
	/*[Client Prediction]*/
	UPROPERTY(BlueprintReadOnly, Category = "Ability")
		float CurrentCastTime;
	/*[Client Prediction]*/
	UPROPERTY(BlueprintReadOnly, Category = "Ability")
		float CurrentCooldownTime;
	/*[Client Prediction]*/
	UPROPERTY(BlueprintReadOnly, Category = "Ability")
		bool IsOnCooldown;
	/*[Client Prediction]*/
	UPROPERTY(BlueprintReadOnly, Category = "Ability")
		bool IsBeingUsed;
	/*
		Default action state component. All actions, like charing, shooting, casting, for
		weapons, abilities, and anything that need some kind of state handling
		will be done trough this component.
	*/
	/*[Replicated]*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		TSubobjectPtr<class UARActionStateComponent> ActionState;

};



