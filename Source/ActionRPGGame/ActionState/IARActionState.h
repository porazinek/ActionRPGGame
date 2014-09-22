// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "IARActionState.generated.h"

UINTERFACE(MinimalAPI)
class UIARActionState : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IIARActionState
{
	GENERATED_IINTERFACE_BODY()

	/*
		For safety reasons below events should ONLY be called from server functions!
	*/
	/*[Server]*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
		void ServerOnActionStart();
	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
		void ServerOnActionStop();
	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
		void ClientOnActionStart();
	/*[Server]*/
	/*
		Called when Input is pressed.
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
		void OnCastStart();
	/*
		Called when Input is released OR when current State ended.
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
		void OnCastEnd();
	/*
		Called when current state is exectued. Usually while button is pressed.
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
		void OnActionInterval();
	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
		void OnCooldownEnd();
	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
		void OnCooldownBegin();
	/*[Server]*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
		void OnActionEnd();
	/*
		Action is prepared. It means we have equiped it to slot, where we can fire it,
		but we did not do so yet.
		Useful when you need to prepare some things before action is used.
		Like Draw targeting circle, or throwing arc.
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
		void OnActionPrepared();

	/*[Client]*/
	UFUNCTION()
		virtual void InputPressed();
	/*[Client]*/
	UFUNCTION()
		virtual void InputReleased();

	UFUNCTION()
		virtual void ActionReload();

	inline TWeakObjectPtr<class AARWeapon> GetCurrentWeapon() { return CurrentWeapon; };
	inline void SetCurrentWeapon(class AARWeapon* WeaponIn) { CurrentWeapon = WeaponIn; };
private:
		TWeakObjectPtr<class AARWeapon> CurrentWeapon;
};
