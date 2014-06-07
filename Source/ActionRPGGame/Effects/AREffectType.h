// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "AREffectType.generated.h"

/*
	EffectType Is NEVER replicated to client. It is always run on server.
	It serve as more advanced data holder class.
	The advanced part is the fact that EffectType can be and should be extended using Blueprint
	Event Graph, to perform operations based of upcoming data, from actor which
	effect is appiled.


	Only the results of effect action are replicated back to client. Which are on actor. 
	Like damage taken, healing, attribute reduction, etc.
*/

UCLASS(minimalapi)
class UAREffectType : public UObject
{
	GENERATED_UCLASS_BODY()


	/*
		For example. We have projectile shooter by player1 and that projectile hit player2. 
		Chain look like this:
		Player1 - EffectInstigator
		Projectile - EffectCausedBy
		Player2 - EffectTarget

		It's important to remember it, as it might be confusuion when trying to access 
		some properties to calculate what effect is doing. You must remember to access
		the properties of the right actor, otherwise it might cause hard to find logical
		errors!
	*/

	/*
		Who will be affected by this effect;
	*/
	UPROPERTY()
		TWeakObjectPtr<AActor> EffectTarget;
	/*
		What caused this effect to EffectTargetl
	*/
	UPROPERTY()
		TWeakObjectPtr<AActor> EffectCausedBy;
	/*
		Who Instigated this effect.
	*/
	UPROPERTY()
		TWeakObjectPtr<AActor> EffectInstigator;
	/*
		Get World from EffectTarget
	*/
	virtual class UWorld* GetWorld() const OVERRIDE;
};



