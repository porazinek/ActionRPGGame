// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../Types/ARAttributeTypes.h"
#include "AREffect.generated.h"

/*
	EffectType Is NEVER replicated to client. It is always run on server.
	It serve as more advanced data holder class.
	The advanced part is the fact that EffectType can be and should be extended using Blueprint
	Event Graph, to perform operations based of upcoming data, from actor which
	effect is appiled.

	Only the results of effect action are replicated back to client. Which are on actor. 
	Like damage taken, healing, attribute reduction, etc.

	Effects can modify anything (in theory at least..) in game to which:
	1. They can get access.
	2. Object they want to modify provide events, which will indicate that it can be modified in some way.

	Current issues:
	1. Two effects can't really modify the same property on single object. It might lead to unpredictable
	results, and more often than not only last of modification iwll be available.
	It possible to somewhat workaround it you know for 100% order of exectution of effects,
	in which case there is not way to reliably predict it right now ;).
	2. Setup in blueprint is currently revolving around using Multicast Delegate Events,
	and require quite a bit of binding steps to make it work. I hope to somewhat simplify it.

	3. Infinite lasting effects will probably need their own subclass, as they have pretty distinctive use
	case. Like feats or traits, which can modify abilities, attributes weapons or items.
*/

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDMDOnEffectInitialized);

UCLASS(Blueprintable, BlueprintType)
class UAREffect : public UObject
{
	GENERATED_UCLASS_BODY()
public:
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
	UPROPERTY(BlueprintReadOnly, Category = "Ownership")
		TWeakObjectPtr<AActor> EffectTarget;
	/*
		What caused this effect to EffectTarget
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Ownership")
		TWeakObjectPtr<AActor> EffectCausedBy;
	/*
		Who Instigated this effect.
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Ownership")
		TWeakObjectPtr<AActor> EffectInstigator;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
		FGameplayTagContainer OwnerTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
		FGameplayTagContainer RequiredTags;

	UFUNCTION()
		virtual void InitializeEffect();

	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly)
		void OnPreInitializeEffect();

	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly)
		void OnEffectInitialized(const AActor* ActorOut);

	//UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Effect")
	//	FDMDOnEffectInitialized OnEffectInit;
public:
	/*
		Get World from EffectTarget
	*/
	virtual class UWorld* GetWorld() const override;
};



