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
	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
		void OnActionInterval();
	/*[Server]*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
		void OnActionEnd();

	/*[Client]*/
	UFUNCTION()
		virtual void InputPressed();
	/*[Client]*/
	UFUNCTION()
		virtual void InputReleased();
};
