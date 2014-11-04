// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../Types/ARAttributeTypes.h"
#include "AREffect.h"
#include "ARFeat.generated.h"

/*
	Feats, are global (in scale of player anyway) effects, which are always active, and passively listen
	for events, to which they can respond.
*/

UCLASS(Blueprintable, BlueprintType, DefaultToInstanced, Within = ARFeatComponent)
class UARFeat : public UAREffect
{
	GENERATED_UCLASS_BODY()
public:

	virtual void InitializeEffect() override;
	/*
		By default, we will create all feats, when player make character.
		But, they are not active by default, which means, they can't affect anything that player does.
		So from player perspective these feats, were never created not choosed by player.
		If player buy any feat, we simply mark this flag as true.
	*/
	/*
		Is feat active ?
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR|Feats")
		bool IsFeatActive;


	UPROPERTY()
	class UARAttributeComponent* Attributes;


	UFUNCTION(BlueprintNativeEvent, Category = "Damage")
		void ModifyOutgoingDamage(float DamageIn, AActor* ModdedActor);

	UFUNCTION(BlueprintNativeEvent, Category = "Actor")
		void ModifyIncomingActor(AActor* ActorIn);

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
		UARFeatComponent* Feats;

};



