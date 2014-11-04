// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "../Types/ARStructTypes.h"
#include "../Types/ARItemTypes.h"
#include "../Types//AREnumTypes.h"
#include "ARItemUpgrade.generated.h"

/*
	Item Upgrade is Object which lives within particular item, and modify it.
	We can split Item Upgrades into two generic categories:
	1. For item, which means it will work with any item.
	2. Per item. It will work only with particular item type, or even subtype.

	The general principle is that these object will modify properties on item (which means they are completly passive).
	Once upgrade class is added to item, it's spawned as object. It will modify properties on item
	and store original values.
	Once upgrade is removed (or replaced), original values will be restored to item which owned upgrade.
	Allowing multiple passive upgrades, is crucial and it will happen I just don't know yet how exactly.

	And there are active upgrades which can mean few things and I probabaly should think of better name ;).
	Active upgrades can be split intwo two general cateogries:
	Passive:
	Item Upgrade will be spawned on item, and listen for incoming events. When there is event to which 
	item can respon, item upgrade will modify it.
	For example you might upgrade your gun to fire with immolation bullets.
	When you press fire, upgrade will intercept incoming input (or shoot event, it depends on particular
	implementation), and add burning damage to each shot.
	Possible implementation is that event intercept target, and when there is shot it spawn perdiodic effect
	burning on target. It's easy to imepelemnt on instant hit actions/weapons.
	For projectile type action, I will have to think of something.
	
	Another category of passive upgrades, are the ones on timers. Effect might be activated by event, 
	or might be working all the time. In either case timer will manage how often effect tick or how long
	it can last before going on cooldown.

	Active:
	You must make action on your own to active effect on item. (for upgrade can provide +100 to damage 
	for 10s, but you must activate it).


	Well. At least. That's theory.

	Upgrades should be in general created and extended by Blueprints. So when you create new C++ based upgrade
	make sure it's easily moddable or extendable by blueprints!
*/
UCLASS(BlueprintType, Blueprintable, DefaultToInstanced, editinlinenew)
class UARItemUpgrade : public UObject
{
	GENERATED_UCLASS_BODY()
};



