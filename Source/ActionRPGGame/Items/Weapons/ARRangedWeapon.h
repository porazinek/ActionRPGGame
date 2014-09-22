// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../ARItem.h"
#include "../ARWeapon.h"
#include "../ActionState/IARActionState.h"

#include "../CosmeticEffects/IARCosmeticEffects.h"

#include "../Types/ARAttributeTypes.h"
#include "../Types/AREnumTypes.h"

#include "ARRangedWeapon.generated.h"

UENUM(BlueprintType)
namespace EWeaponReloadType
{
	enum Type
	{
		EntireMagainze,
		SingleBullet,

		MAX
	};
}

/*
	Assume that every Ranged weapon, required some kind of ammunition. Energy, projectiles,
	throwing stones ;).
*/
UCLASS(minimalapi)
class AARRangedWeapon : public AARWeapon, public IIARCosmeticEffects
{
	GENERATED_UCLASS_BODY()
public:
	virtual void BeginPlay() override;

	/** IIARActionState Interface - Begin */

	virtual void InputPressed() override;

	virtual void InputReleased() override;

	virtual void ActionReload() override;
	/** IIARActionState Interface - End */



	/** IIARCosmeticEffects - Begin */

	virtual FVector GetOriginLocation() override;

	/** IIARCosmeticEffects - End */


	UPROPERTY(EditAnywhere, Category = "Cosmetics")
		FName MuzzleSocket;

	/*
		Total, maximum amount of amunition for this weapon.
	*/
	UPROPERTY(EditAnywhere, Category = "Ammo")
		int32 MaxAmmo;

	UPROPERTY(BlueprintReadWrite, Category="Ammo")
		int32 CurrentAmmo;

	/*
		Amount of ammunition consumned per single shot.
	*/
	UPROPERTY(EditAnywhere, Category = "Ammo")
		int32 AmmoPerShot;

	/*
		Maximum ammo in single magazine.
	*/
	UPROPERTY(EditAnywhere, Category = "Ammo")
		int32 MaxMagazineSize;

	/*
		Current amount of ammunition in magazine.
	*/
	UPROPERTY(BlueprintReadWrite, Category = "Ammo")
		int32 CurrentMagazineSize;

	UPROPERTY(EditAnywhere, Category = "Firing")
		float ReloadTime;

	/*
		Interval between shots, when continusly fire
	*/
	UPROPERTY(EditAnywhere, Category = "Firing")
		float RefireSpeed;
	/*
		When button will be released, how long do I wait before can shoot again.
		Most of the time the same RefireSpeed.
	*/
	UPROPERTY(EditAnywhere, Category = "Firing")
		float RefireCooldown;

	UPROPERTY(EditAnywhere, Category = "Firing")
		TEnumAsByte<EWeaponReloadType::Type> ReloadType;

	void StartFire();
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerStartFire();

	void StopFire();

	bool CheckAmmo();
	void SubtractAmmo();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerReloadMagazine();
	void ReloadMagainze();


	UFUNCTION()
		void OnFiring();
};



