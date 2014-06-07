ActionRPGGame
=============

Unreal Engine 4 starter kit for Action RPG type game.

1. It containes very little graphical content.
2. Supports replication. 
3. It's designed to be as easy as possible to extend and build upon. There are some non-game specific systems, and there game specific systems. Please note that integrity of sample won't be sacrificed, to make something more generic. If you don't like some parts of code, you will have just change them ;).
4. This design focuses on exposing as much as functionality back to blueprint. For easy creation of things like items, weapons, spells, abilities and pretty much other active gameplay elements.


If you want use this code with existing project or with new one, remember that you must edit name of class in Public/ folder. You will also need to change include name of this class in all implementation files.

Source commits to this repo, should at least compile. But I don't guarantee they will work.

What is in now:

1. Component Based Action State Machine. As a component it can be attached to any actor. It doesn't pose one requirment on actor. Actor must implement, special interface, that have events needed to bind events from component as well as basic Input function. You add it to any actor and then you can:

a) Create casting based weapons/abilities. User press button and cast begin, after certain amount of time, event is fiered.

b) Add cooldown. For spell it can be just recast cooldown, in case of weapon it might ammo reload. 

c) You can add other funcionality easily.

States run completly on server and are never replicated back to client. So if you want to make smooth cooldown timer or cast time, you will need to build client side prediction on your own. Kit provides sample implementation.


Things to do:

1. Attributes.

2. Effects. The essentialy are appiled to actor and apply attribute modification based on Instigator, Casuer or Target attributes. They are scriptied in blueprints.

3. Basic Equipment system.

4. Weapons. Sample for mele weapon, bow, gun.
