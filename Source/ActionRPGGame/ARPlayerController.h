// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "Types/ARStructTypes.h"

#include "Types/ARInvItem.h"

#include "Slate.h"
#include "Componenets/ARAttributeComponent.h"
#include "ARCharacter.h"

#include "ARPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDMDOnAbilityEquiped, class AARAbility*, Ability);

UCLASS()
class AARPlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()
private:
	UPROPERTY()
	class AARCharacter* ARCharacter;
public:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		TSubobjectPtr<class UARInventoryComponent> Inventory;

	UPROPERTY(ReplicatedUsing=OnRep_UIDamage)
	FARUIDamage UIDamage;
	UFUNCTION()
		void OnRep_UIDamage();
	/*
	Hook for controller. Controller doesn't have any attributes per se,
	but attribute component also implements notifications and events,
	and we'd like to access them from owning conroller as well.

	Update:
	I'm not sure if I'm going to keep it here. It's way to confusing.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
		TSubobjectPtr<class UARAttributeBaseComponent> Attributes;
	virtual void SetPawn(APawn* InPawn) override;

	virtual void PostInitializeComponents() override;

	/* Input **/
	virtual void SetupInputComponent() override;
	void InputActionButtonOne();
	void InputActionButtonTwo();

	template<int Index>
	void InputActionBar()
	{
		if (ActionBarOne[Index].Ability.IsValid())
		{
			IIARActionState* actionInterface = InterfaceCast<IIARActionState>(ActionBarOne[Index].Ability.Get());
			if (actionInterface)
			{
				actionInterface->InputPressed();
			}
		}
	}

	void InputActivateAbility();
	/* 
		Route weapon fire trough Controller
		Will be useful, when later there will be multiple pawns.
		Like vechicles, so we can just use the same binding to fire vechicle actions.
	*/
	void InputFireLeftWeapon();
	void InputFireRightWeapon();
	void InputStopFireLeftWeapon();
	void InputStopFireRightWeapon();

	void InputTempAddWeapons();

	void InputSwapLeftWeapon();
	void InputSwapRightWeapon();

	/*
		Route item picup trough controller.
		We might pick up directly in controller, or route to other object.
	*/
	void InputPickupItem();

	/* GUI Input **/
	void SetInventoryVisibility();
	EVisibility InventoryVisibility;

	void SetCharacterSheetVisibility();
	EVisibility CharacterSheetVisibility;

	void SetAbilityInventoryVisibility();
	EVisibility AbilityInventoryVisibility;

	/*
		Temporary test for Effects which are going to be feats.
		Base functionality test.
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Feats")
		TArray<class UAREffectType*> Feats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feats")
		TArray<TSubclassOf<class UAREffectType>> FeatClasses;
	/*
		Everything regarding currently equiped items and inventory is handled on server.
		Every time user is trying to change something in inventory there is RPC call made from client 
		to server, where client request operation on Item.

		It could potentialy get quite heavy, because all RPC calls are reliable,
		My assumption here is, that operations on inventory in most extreme cases, are not
		that often, and we don't want risk user operation on his own items will be lost
		somewhere over network.
	*/

	/* 
		Ability Inventory - Spellbook, mele action bla bla, got the point 
	**/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_AbilityInventory, Category = "Abilities")
		TArray <FAbilityInfo> AbilityInventory;
	UFUNCTION()
		void OnRep_AbilityInventory();
		bool UpdateAbilityInventory;
	/*
		Abilities

		this is going to seprate component. Later.
	*/
	/*
		Array which represents action bar. Essentialy hack, it would be better as TMap,
		but by default TMap is not exposed to reflection and replication.
	*/
	UPROPERTY(ReplicatedUsing = OnRep_ActionBarOne)
		TArray<FAbilityInfo> ActionBarOne;
	UFUNCTION()
		void OnRep_ActionBarOne();
	bool UpdateActionBarOne;

	UPROPERTY(Replicated, BlueprintReadOnly, Replicated, Category="Ability")
	class AARAbility* ActiveAbility;

	bool AddAbilityToInventory(FAbilityInfo AbilityIn);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAddAbilityToInventory(FAbilityInfo AbilityIn);

	UFUNCTION(BlueprintCallable, Category = "Ability")
		void AddAbilityToActionBar(FAbilityInfo AbilityIn, int32 SlotID);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAddAbilityToActionBar(FAbilityInfo AbilityIn, int32 SlotID);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetActiveAbility(class AARAbility* AbilityIn);
	UFUNCTION()
		void SetActiveAbility(class AARAbility* AbilityIn);
	/*
		Event called when ability is aded to action bar.
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Ability")
		FDMDOnAbilityEquiped OnAbilityEquiped;

};



