// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "ARActionStateComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionCastBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionCastEnd);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCooldownBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCooldownEnded);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionInterval);

UCLASS(meta = (BlueprintSpawnableComponent), hidecategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming))
class UARActionStateComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

		virtual void InitializeComponent() override;

	UPROPERTY(Replicated)
	bool BlankRep;

	virtual void TickMe(float DeltaTime);
	/*
		this shouldn't be needed but editinlinenew doesn't work.
	*/
	UPROPERTY(EditAnywhere, Category = "State")
		TSubclassOf<class UARActionState> ActionStateClass;

	UPROPERTY(Instanced, BlueprintReadOnly, Category = "State")
		TSubobjectPtr<class UARActionState> ActiveState;
	UPROPERTY(Instanced, BlueprintReadOnly, Category = "State")
		TSubobjectPtr<class UARActionState> CooldownState;

	/*
		[server] State we are currently in ReplicatedUsing=OnRep_CurrentState
	*/
	UPROPERTY()
		class UARActionState* CurrentState;
	//UFUNCTION()
	//	void OnRep_CurrentState();
	//void SetState(class UARActionState* StateIn);
	/*
		[server] ActionState. What will happen when we receive input.
	*/
	UPROPERTY(Instanced, EditAnywhere, BlueprintReadWrite, EditFixedSize, Category = "State")
	class UARActionState* StartActionState;

	/*
		[server] go to some state.
	*/
	void GotoState(class UARActionState* NextState);

	/*
		[server] Begin Sequence of CurrentState.	
	*/
	virtual void BeginActionSequence();
	/*
		[server] State can call this function to fire action.
	*/
	virtual void FireAction();
	
	/*
		How much time before action will be Fired.
		Can be used in automatic or charged actions.
	*/
	UPROPERTY(EditAnywhere, Category = "Action Usage")
		float MaxCastTime;
	/*
		How often should action be perfomed
	*/
	UPROPERTY(EditAnywhere, Category = "Action Usage")
		float IntervalTime;
	/*
		How much time before action can be used again.
		For example weapon ammo reaload or RPG type ability recast cooldown.
	*/
	UPROPERTY(EditAnywhere, Category = "Action Usage")
		float ActionCooldownTime;

	UPROPERTY(BlueprintReadOnly, Category = "Action Usage")
		float CurrentCastTime;
	UPROPERTY(BlueprintReadOnly, Category = "Action Usage")
		float CurrentCooldownTime;

	/*[client] - [server]*/
	UFUNCTION()
		virtual void StartAction();
	UFUNCTION(Server, Reliable, WithValidation)
		virtual void ServerStartAction();

	/*
		These are used to call Delegates from withing states.
		You can overridem to add more funcionality. 
		Remember to add super:: or make sure you call delegate on your own when override!
	*/

	/*[Server]*/
	virtual void CooldownEnded();

	/*[Server]*/
	virtual void CooldownBegin();

	/*[Server]*/
	virtual void CastBegin();

	/*[Server]*/
	virtual void CastEnd();

	/*[Server]*/
	virtual void ActionInterval();

	/*
		Helper events delgates, which might be useful, when defining weapon, ability, spells, etc.
		in blueprints.
	*/

	/*[server] - called when casting is started */
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Action State")
		FOnActionCastBegin OnActionCastBegin;
	/*[server] - Called when casting is finished */
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Action State")
		FOnActionCastEnd OnActionCastEnd;
	/*[server] - called on each action interval */
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Action State")
		FOnActionInterval OnActionInterval;
	/*[server] - called when cooldown begin */
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Action State")
		FOnCooldownBegin OnCooldownBegin;
	/*[Server] - called when cooldown ends */
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Action State")
		FOnCooldownEnded OnCooldownEnded;
};



