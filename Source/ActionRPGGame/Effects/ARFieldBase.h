// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../Attributes/IAttribute.h"
#include "GameplayTagAssetInterface.h"

#include "../Types/ARAttributeTypes.h"

#include "ARFieldBase.generated.h"

USTRUCT(BlueprintType)
struct FFieldTags
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "Tags")
		FGameplayTagContainer OwnedTags;
	UPROPERTY(BlueprintReadOnly, Category = "Tags")
		FGameplayTagContainer RequiredTags;
};

/*
	Questions:
	1. Should Finishers spawn their own comsetic effects ?
	2. If so which ones ?
	3. Or should they just be triggered, and effect of whatever ability/weapon hit field
	will be used ?
	4. Combine both ?
	5. Blast finisher doesn't really need it's own effect.
	6. Neither finisher which spawn other actors. We can assume, that spawned actors
	will cover whatever visuals are needed.
*/
/*
	Blast Finisher Data Structure
*/
USTRUCT(BlueprintType)
struct FBlastFinisherData
{
	GENERATED_USTRUCT_BODY()
public:
	/*
		Radius of Blast
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blast Finisher")
		float Radius;
	/*
		Damage to apply after Blast. Might be left to 0;
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blast Finisher")
		FAttribute Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blast Finisher")
		FGameplayTagContainer BlastDamageTags;

	/*
		Damage which is appiled to field after each blast.
		If field Attribute reaches zero, it is destroyed.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blast Finisher")
		FAttribute DamageToField;

	/*
		Effects to apply after Blast. Might be 0;
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blast Finisher")
		TArray<class UAREffectType*> EffectsToApply;

	/*
		Whether to remove field after blast combo. In most cases false;
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blast Finisher")
		bool bRemoveField;

	/*
		Incoming Damage must have this tag to trigger Blast Finisher.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blast Finisher")
		FGameplayTag BlastFinisherTag;
};

USTRUCT(BlueprintType)
struct FDirectionalImpulseFinisherData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Directional Impulse Finisher")
		float Density;
};

UENUM(BlueprintType)
enum EFieldComboType
{
	ComboBase,
	ComboFinisher
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FDMDOnBeginFieldOverlap, FFieldTags, OtherTags, class AActor*, OtherActor, bool, bFromSweep, const FHitResult &, SweepResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDMDOnFieldsOverlap, FFieldTags, OtherTags, class AARFieldBase*, OtherField);
/*
	Base class for Field.
	Field is special actor which can be spawned by other actor and which presist in game world:
	1. Until it's life time expire.
	2. Until it takes to much damage.
	3. Until it is triggered. (like proximity mine).
	4. Other conditions might be defined as needed.

	Fields can interact with other fields. For example fire field can be extinguished by wind (or spread by it)
	or by water.
	Oil field can be burned by fire, etc.

	Fields are visible by everyone, if they have any visible componenets (Particle effects, meshes etc).

	Fields can interact with abilities and weapons.

	Fields should be moveable by Abilities, weapon and other fields.
	For example we have acid fog field. It is gas that hang in area.
	Now we use ability which create directional wind effect. Field should move in direction of wind.
	(or other Force push direction).

	Another possibility is that fog will be affected by omnidirectional impulse. 
	In that case effective radius of field would increase, 
	but density decrease, which means less damage but over large area.

	Vortex - would pull fog from level.
	Vortex might expole, so after sucking fog, after explosion it will be spread again.

	These interactions should be made as automatic as possible, and 
	just expose properties back to blueprint.

*/

UCLASS()
class AARFieldBase : public AActor, public IIAttribute, public IGameplayTagAssetInterface
{
	GENERATED_UCLASS_BODY()
public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void Initialize();

	/*
		IIAttribute interface.
	*/
	virtual void Died() override;

	virtual void OnRecivedDamage_Implementation(FAttributeChanged AttributeChanged, FGameplayTagContainer DamageTag) override;
	
	virtual void OnRecivedRadialDamage_Implementation(FAttributeChanged AttributeChanged, FARRadialDamageEvent const& Damage, FGameplayTagContainer DamageTag) override;
	/*
		IGameplayTagAssetInterface
	*/
	/* copy-paste from GameplayTagAssetInterface.h	*/
	/**
	* Get any owned gameplay tags on the asset
	*
	* @param OutTags	[OUT] Set of tags on the asset
	*/
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	/**
	* Check if the asset has a gameplay tag that matches against the specified tag (expands to include parents of asset tags)
	*
	* @param TagToCheck	Tag to check for a match
	*
	* @return True if the asset has a gameplay tag that matches, false if not
	*/
	UFUNCTION(BlueprintCallable, Category = GameplayTags)
		virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;

	/**
	* Check if the asset has gameplay tags that matches against all of the specified tags (expands to include parents of asset tags)
	*
	* @param TagContainer			Tag container to check for a match
	* @param bCountEmptyAsMatch	If true, the parameter tag container will count as matching, even if it's empty
	*
	* @return True if the asset has matches all of the gameplay tags
	*/
	UFUNCTION(BlueprintCallable, Category = GameplayTags)
		virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer, bool bCountEmptyAsMatch = true) const override;

	/**
	* Check if the asset has gameplay tags that matches against any of the specified tags (expands to include parents of asset tags)
	*
	* @param TagContainer			Tag container to check for a match
	* @param bCountEmptyAsMatch	If true, the parameter tag container will count as matching, even if it's empty
	*
	* @return True if the asset has matches any of the gameplay tags
	*/
	UFUNCTION(BlueprintCallable, Category = GameplayTags)
		virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer, bool bCountEmptyAsMatch = true) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Componenets")
		TSubobjectPtr<class USphereComponent> Collision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componenets")
		TSubobjectPtr<class UARFieldAttributes> Attributes;

	//How much time must pass between ticks.
	//Usefull for applying damage or area search (SweepMulti) to check
	//if any actors are within field radius.
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Properties")
		float TickInterval;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Properties")
		bool IsTicking;

	/*
		Maxium life time in seconds.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Properties")
		float MaximumLifeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Properties")
		bool IsLifeInfinite;

	/*
		If true, field can be combined with other field only if these two
		have the same owner.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Properties")
		bool MustHaveSameOwnerToCombine;

	/*
		Can this field be used as part of combo ?
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Properties")
		bool CanBeCombined;

	/*
		ComboBase - must be placed first.
		ComboFinisher - must be placed over ComboBase.
	*/
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Properties")
	//	TEnumAsByte<EFieldComboType> FieldComboType;

	/*
		Because for whatever reason default instigator is now APawn in AActor 0_o.
	*/
	UPROPERTY(BlueprintReadWrite, Category = "Ownership")
		AController* InstigatorController;

	/*
		My Tags
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
		FGameplayTagContainer OwnedTags;
	/*
		Tags I require on other field to make any interaction with it.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
		FGameplayTagContainer RequiredTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
		FGameplayTag BlastTag;

	//UPROPERTY(BlueprintReadOnly, Category = "Field")
	//	TArray<class AARCharacter*> CharactersInField;

	/*
		Event Delgate that contain information about what overlaped our field.
	*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Field")
		FDMDOnBeginFieldOverlap OnBeginFieldOverlap;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Field")
		FDMDOnFieldsOverlap OnFieldsOverlap;

	UFUNCTION(BlueprintImplementableEvent, Category = "Field")
		void OnFieldInitialized();

	UFUNCTION(BlueprintImplementableEvent, Category = "Field")
		void OnFieldBeginDestroy();

	UFUNCTION(BlueprintImplementableEvent, Category = "Field")
		void OnFieldTick();

	/**
		This Event is called when certain condition of TWO overlaping fields are meet.
		First Owned and Required tags in respective fields must match.
		Then we check whther or not that fields requre to be spawned by they same owner.

		At this point we call this event.

		Intended usage is that with Call Parent function, you will provide class of this new field
		and all needed parameters.
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "Field")
		void OnCombineFields(TSubclassOf<class AARFieldBase> NewField, float DurationIn, float TickIntervalIn);

	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly, Category = "Field")
		void OnReceiveFieldsOverlap(FFieldTags OtherTags, class AARFieldBase* OtherField);


	/*
		Spawn combo field in place of current one. 
		Use to create combos of two overlapping fields.
	*/
	UFUNCTION(BlueprintCallable, Category = "AR|Field")
		void SpawnField(TSubclassOf<class AARFieldBase> Field, float Duration, float TickIntervalIn, FVector Location, AARFieldBase* OtherField);
	
	/*
		Combine two fields. 
		First field is the on on which the function is called.
		Second field filed is coming from overlap event.

		Currently you must check tags in blueprint. Something to improve.

		bDestroyOldFields - Indicates whether fields used to combo should stay in world
		after combo is successful, or not.
	*/
	UFUNCTION(BlueprintCallable, Category = "AR|Field")
		void CombineFields(TSubclassOf<class AARFieldBase> NewField, float DurationIn, float TickIntervalIn, AARFieldBase* OtherFieldIn, bool bDestroyOldFields = true);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Finisher Data")
		FBlastFinisherData BlastFinisherInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Finisher Data")
		FDirectionalImpulseFinisherData DirectImpulseFinisherInfo;
	/*
		Called When blast finisher is executed. I dunno why would it be needed but..
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Field")
		void OnBlastFinisher();

	/*
		This will spawn projectiles in random horizontal direction.
		It's actually might another field that presist in world, and while with this field
		it will shoot projectiles. (like Tornado)

		Might be ability as well like whirlwind.
	*/
	UFUNCTION(BlueprintCallable, Category = "AR|Field")
		void WhirlFinisher();
	/*
		Blast Finisher can be only be triggered by Radial damage.
	*/
	UFUNCTION()
		void BlastFinisher(FAttributeChanged const& DamageInfo, FARRadialDamageEvent const& HitInfo);

	/*
		When field is overlaped by any heat source,
		it will explode dealing more damage than normaly in instant.
	*/
	UFUNCTION()
		void CombustionFinisher();

	/*
		Two below functions need some work and thought.
		They should take impact point into consideration and strenght of impule
		to deterimne how do spread/push field.
		We don't really want to just push spehere from one point to another, or 
		increase sphere radius.

		Best solution would be to spawn another field, in direction of current one.
		These two would overrlap and share all properties (timers, health etc).

		We might need another actor like. Multifield actor, which would contain list of all fields.
		And would take care of their positions and health.
	*/
	/*
		TMap ?
	*/
	/*
		Child fields of this field. Childs fields:
		1. Share the same type.
		2. Share the same effects.
		3. Might have different durations.
		4. Actor with child can't be dstroyed as long as there are childs.
		Or maybe it will be destroyed and child list could be copied to one of childs ?

	*/
	UPROPERTY()
		TArray<class AARFieldBase*> ChildFields;

	/*
		Will push field in view direction of actor, which triggered this Finisher
	*/
	UFUNCTION()
		void DirectionalImpulseFinisher();
	/*
		Will push field in all direction. Essentialy increasing it's radius.
	*/
	UFUNCTION()
		void OmniImpuleFinisher();

	void ClearTimers();
	void Deactivate();
private:

	float CurrentLifeTime;
	FTimerHandle LifeHandle;
	FTimerHandle IntervalHandle;

	/*
		Helper to set values in OnBeginFieldOverlap delegate
	*/
	UFUNCTION()
		void SetOnBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void SetOnFieldsOverlap(FFieldTags OtherFieldTagsIn, class AARFieldBase* OtherField);

	void SelfClean();

	void SetFieldTick();
};



