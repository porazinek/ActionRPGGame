// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../Types/ARAttributeTypes.h"
#include "ARFeatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDMDOnOutgoingDamager, float, AtributeMod, AActor*, ModdedActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDMDOnIncomingActor, AActor*, ActorIn);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_RetVal_OneParam(float, FDMMROnOutgoingDamage, float, DamageIn);
/*
	This component, allow any actor to deal damage to other actor.
	But doesn't allow owning actor to receive any damage.
	
	It will just take base damage value, modify it with other active objects, and then
	spit out damage, which can be modified by other componenets, functions etc.
*/
UCLASS(hidecategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming), editinlinenew, meta = (BlueprintSpawnableComponent))
class UARFeatComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()
public:

	virtual void InitializeComponent() override;

	UPROPERTY(EditAnywhere, Category = "Feats")
		TArray<TSubclassOf<class UARFeat> > FeatClasses;

	UPROPERTY()
		TArray<UARFeat*> ActiveFeats;

	UPROPERTY()
	class UARAttributeComponent* Attributes;

	///UPROPERTY()
	///	FDMDOnOutgoingDamager OnOutgoingDamage;
	UPROPERTY()
		FDMDOnOutgoingDamager OnOutgoingDamage;

	UPROPERTY()
		FDMDOnIncomingActor OnIncomingActor;

	UPROPERTY(BlueprintReadWrite, Category = "Damage")
		float CachedDamage;
};
