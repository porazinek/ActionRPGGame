// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "../ActionState/IARActionState.h"

#include "../CosmeticEffects/IARCosmeticEffects.h"

#include "../Types/ARStructTypes.h"

#include "ARAbility.generated.h"

UENUM()
enum EAbilityOrigin
{
	OpositeHandToWeapon,
	UseWeaponLocation
};
/*
	Probabaly need to split abilities into more descriptive and contained classes. Like
	Mele, Ranged, Spell, etc. Where it makes sense of course.
*/
UCLASS(hidecategories = (Input, Movement, Collision, Rendering, "Utilities|Transformation"), MinimalAPI, Blueprintable, notplaceable, DefaultToInstanced)
class AARAbility : public AActor, public IIARActionState, public IIARCosmeticEffects
{
	GENERATED_UCLASS_BODY()
public:
	virtual void Tick(float DeltaSeconds) override;

	//Mainly used on server. To assign Owner properties;
	virtual void Initialize();

	virtual void BeginPlay() override;

	/* 
		Currently Active weapon, must have any of these tags.
		If it doesn, then this ability can't be used 
	**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
		FGameplayTagContainer WeaponRequiredTags;

	/*
		These tags will be mainly used with Effects (which can be premament like Feats, Traits, Skills
		or temporary like buffs), so effects can respond to events,
		when particular ability is used. Don't put to much tags here! Only absolute nesseary ones.
		If needed just create new ones.
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
		FGameplayTagContainer OwnedTags;

	UPROPERTY(EditAnywhere, Category = "GUI")
		FSlateBrush AbilityIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Casting Speed")
		float CastingSpeed;

	UPROPERTY(EditAnywhere, Category = "Cost")
		TArray<FAttribute> ResourceCost;
	/*
		Does ability require two weapons to be activate ?
	*/
	UPROPERTY(EditAnywhere, Category = "Reuired Weapons")
		bool MustHaveTwoWeaponActive;

	/*
		Three properties below are actually only usefull for ranged abilities.
		For mele too, if you really want to spawn some effect from socket for whatever reason.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Cosmetics")
		FName LeftHandSocket;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Cosmetics")
		FName RightHandSocket;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Cosmetics")
		FName WeaponSocket;
	/*
		Origin point will be in opposite Hand, to the one currently holding weapon.
	*/
	//UPROPERTY(EditAnywhere, Category = "Cosmetics")
	//	bool OpositeHandToWeapon;
	/*
		Origin will be on weapon socket.
	*/
	//UPROPERTY(EditAnywhere, Category = "Cosmetics")
	//	bool UseWeaponLocation;
	UPROPERTY(EditAnywhere, Category = "Cosmetics")
		TEnumAsByte<EAbilityOrigin>  StartLocation;

	//UFUNCTION(BlueprintCallable, Category="AR|Ability")
	//	FVector GetOriginLocation();

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Ownership")
	class AARCharacter* OwningCharacter;
	UPROPERTY()
	class AARPlayerController* OwiningController;
	UPROPERTY()
	class UARAttributeComponent* OwnerAttributes;
	UPROPERTY()
	class UAREquipmentComponent* OwnerEquipment;

	UPROPERTY(Replicated)
	class AARWeapon* CurrentWeapon;

	UPROPERTY()
	class AARWeapon* LeftWeapon;
	UPROPERTY()
	class AARWeapon* RightWeapon;

	/* [client] OVERIDE from IIARActionState */
	virtual void InputPressed() override;
	virtual void InputReleased() override;


	/** IIARCosmeticEffects - Begin */

	virtual FVector GetOriginLocation() override;

	/** IIARCosmeticEffects - End */


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
public:

	float GetCurrentCastTime();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
		float RechargeTime;

	//CastTime should be at least, as long as animation needed to play, before ability is executed.
	//even instant abilities, have same delay, because of animation ;).
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
		float MaxCastTime;
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
		class UARActionStateComponent* ActionState;

	/* Cosmetics */
	UPROPERTY(EditAnywhere, Category = "Cosmetics")
		UParticleSystem* TrailFX;
protected:
	/*
		Check if player have active weapon required to use this ability,
	*/
	bool CheckWeapon();
	/*
		Check if player have resourced needed to use ability, and subtracts them.
	*/
	bool CheckResources();

	UFUNCTION()
	void OnLeftHandWeapon(class AARWeapon* WeaponIn);
	UFUNCTION()
	void OnRightHandWeapon(class AARWeapon* WeaponIn);
};



