// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"
#include "../Componenets/ARFieldAttributes.h"
#include "../Effects/AREffectType.h"

#include "../Types/ARAttributeTypes.h"

#include "../BlueprintLibrary/AREffectStatics.h"

#include "ARFieldBase.h"

AARFieldBase::AARFieldBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//it might be better to use TimerManager instead. We will see.


	Collision = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("Collision"));
	RootComponent = Collision;
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AARFieldBase::SetOnBeginOverlap);
	//Collision->OnComponentHit
	Attributes = ObjectInitializer.CreateDefaultSubobject<UARFieldAttributes>(this, TEXT("Attributes"));
	//OnFieldsOverlap.AddDynamic(this, &AARFieldBase::SetOnFieldsOverlap);

	bReplicates = true;
	bReplicateInstigator = true;
	bReplicateMovement = true;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;

	CurrentLifeTime = 0;
	IsLifeInfinite = false;

	//bNetTemporary = false;

}
void AARFieldBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!IsLifeInfinite)
	{
		CurrentLifeTime += DeltaSeconds;
		if ((CurrentLifeTime > MaximumLifeTime))
		{
			//this->Destroy();
		}
	}

}

void AARFieldBase::Initialize()
{
	if (Role < ROLE_Authority)
		return;

	SetDeathAttribute("Health");
	OnFieldInitialized();
	if (!IsLifeInfinite)
	{
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();

		FTimerDelegate Del = FTimerDelegate::CreateUObject(this, &AARFieldBase::Died);
		TimerManager.SetTimer(LifeHandle, Del, MaximumLifeTime, false, MaximumLifeTime);
	}

	if (IsTicking)
	{
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		FTimerDelegate TickDelegate = FTimerDelegate::CreateUObject(this, &AARFieldBase::SetFieldTick);
		TimerManager.SetTimer(IntervalHandle, TickDelegate, TickInterval, true);
	}
}
/**
	IIAttribute interface.
	*/
void AARFieldBase::Died()
{
	Deactivate();
	ClearTimers();
	SelfClean();
}
/**
	IGameplayTagAssetInterface
	*/
void AARFieldBase::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = OwnedTags;
}

bool AARFieldBase::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	if (OwnedTags.HasTag(TagToCheck, EGameplayTagMatchType::IncludeParentTags, EGameplayTagMatchType::Explicit))
	{
		return true;
	}
	return false;
}

bool AARFieldBase::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer, bool bCountEmptyAsMatch) const
{
	if (OwnedTags.MatchesAll(TagContainer, bCountEmptyAsMatch))
	{
		return true;
	}
	return false;
}

bool AARFieldBase::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer, bool bCountEmptyAsMatch) const
{
	if (OwnedTags.MatchesAny(TagContainer, bCountEmptyAsMatch))
	{
		return true;
	}
	return false;
}

void AARFieldBase::SpawnField(TSubclassOf<class AARFieldBase> Field, float Duration, float TickIntervalIn, FVector Location, AARFieldBase* OtherField)
{
	if (!Field)
		return;
	/*
		At this point two fields should be overlapping each other.
		Clear their timers to prevent them from executing any actions;
		*/
	this->Died();
	OtherField->Died();
	//ClearTimers();
	//OtherField->ClearTimers();
	/*
		There is needed some grace period when two fields interact with each other.
		Water won't extinguish fire instantly (if at all, there might not be enough of it).
		Oil don't burn instantly over entire surface, instead it will start spreading from impact point.
		etc.

		We could do it as attributes or just properties. Like Fuel, Energy, temperature, etc.

		Now they are just pretty much instantly destroyed and new one is placed.
		*/
	FTransform SpawnTM(FRotator(0, 0, 0), Location);
	AARFieldBase* field = Cast<AARFieldBase>(UGameplayStatics::BeginSpawningActorFromClass(this, Field, SpawnTM, true));
	if (field)
	{
		field->MaximumLifeTime = Duration;
		field->TickInterval = TickInterval;
		field->SetOwner(Instigator);
		field->Initialize();
		//field->Instigator = Instigator;
		UGameplayStatics::FinishSpawningActor(field, SpawnTM);
	}
	//this->SelfClean();
	//OtherField->SelfClean();
}

void AARFieldBase::CombineFields(TSubclassOf<class AARFieldBase> NewField, float DurationIn, float TickIntervalIn, AARFieldBase* OtherFieldIn, bool bDestroyOldFields)
{
	if (!NewField || !OtherFieldIn)
		return;

	if (bDestroyOldFields)
	{
		this->Died();
		OtherFieldIn->Died();
	}

	FTransform SpawnTM(FRotator(0, 0, 0), GetActorLocation());
	AARFieldBase* field = Cast<AARFieldBase>(UGameplayStatics::BeginSpawningActorFromClass(this, NewField, SpawnTM, true));
	if (field)
	{
		field->MaximumLifeTime = DurationIn;
		field->TickInterval = TickInterval;
		field->SetOwner(Instigator);
		field->InstigatorController = InstigatorController;
		field->Initialize();
		//field->Instigator = Instigator;
		UGameplayStatics::FinishSpawningActor(field, SpawnTM);
	}
}

void AARFieldBase::OnRecivedDamage_Implementation(FAttributeChanged AttributeChanged, FARDamageEvent const& Damage, FGameplayTagContainer DamageTag)
{
	//if incoming damage is blast finisher
	//call blast finisher function.
	//but! How do I know which tag is blast finisher ?
	//hardcode tag names - terri bad!
	//create another container like BlastFinisherTags - not that good either.
	//create FGameplayTag ? possibly ?

	//MAYBE! move this functionality out of Field it self to static library function ?
	//Implement it for Weapon and Ability separatetly ?
	//Make interface ?
	//Make Component ?
	//Appiled effects and damage are based on FieldType not ability!
	//which means we would still need to pull off FieldType, and the designer would have to create
	//separate functions for each possible field/finisher combination - uber-super-terri

	//if damage is not combo finisher. There is not reason to proceed any further.
	if (Damage.IsComboFinisher)
	{
		if (DamageTag.HasTag(CombustionFinisherInfo.CombustionTag, EGameplayTagMatchType::IncludeParentTags, EGameplayTagMatchType::Explicit))
		{
			CombustionFinisher(AttributeChanged, Damage);
		}
	}
}

void AARFieldBase::OnRecivedRadialDamage_Implementation(FAttributeChanged AttributeChanged, FARRadialDamageEvent const& Damage, FGameplayTagContainer DamageTag)
{
	if (Damage.IsComboFinisher)
	{
		if (DamageTag.HasTag(BlastFinisherInfo.BlastFinisherTag, EGameplayTagMatchType::IncludeParentTags, EGameplayTagMatchType::Explicit))
		{
			BlastFinisher(AttributeChanged, Damage);
		}
	}
}

void AARFieldBase::OnRecivedLineBoxDamage_Implementation(FAttributeChanged AttributeChanged, FARLineBoxDamageEvent const& Damage, FGameplayTagContainer DamageTag)
{

}
void AARFieldBase::BlastFinisher(FAttributeChanged const& DamageInfo, FARRadialDamageEvent const& HitInfo)
{
	//mod value is different from 0, which means we want to do something with it.
	if (BlastFinisherInfo.Damage.ModValue != 0)
	{
		//Location - should be where the impact point of ability/weapon hit is.
		//Radius - could be radius of incoming damage.
		//DamageType - whatever.
		//Instigator - should be whoever hit this combo field.
		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(this);

		UAREffectStatics::ApplyRadialDamage(BlastFinisherInfo.Damage.AttributeName, 
			BlastFinisherInfo.Damage.ModValue, HitInfo.HitInfo.Location, ECollisionChannel::ECC_Pawn,
			BlastFinisherInfo.Radius, UDamageType::StaticClass(), IgnoredActors, this, DamageInfo.DamageInstigator, true,
			BlastFinisherInfo.BlastDamageTags, false);
	}

	//if there are any effects we will just apply them.
	/*
		Effects need some rework. Until that
		I will not bother implementing this part.
	*/
	for (UAREffectType* effect : BlastFinisherInfo.EffectsToApply)
	{

	}
	OnBlastFinisher();
	if (BlastFinisherInfo.bRemoveField)
	{
		this->Died();
	}
}

void AARFieldBase::WhirlFinisher()
{

}

void AARFieldBase::CombustionFinisher(FAttributeChanged AttributeChanged, FARDamageEvent const& Damage)
{
	OnCombustionFinisher();
}

void AARFieldBase::DirectionalImpulseFinisher()
{

}

void AARFieldBase::OmniImpuleFinisher()
{

}

void AARFieldBase::SetOnBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Role < ROLE_Authority)
		return;

	FFieldTags OtherFieldTags;
	AARFieldBase* OtherField = nullptr;
	if (OtherActor)
	{
		OtherField = Cast<AARFieldBase>(OtherActor);
		if (OtherField)
		{
			OtherFieldTags.OwnedTags = OtherField->OwnedTags;
			OtherFieldTags.RequiredTags = OtherField->RequiredTags;
		}
	}
	if ((OtherField != nullptr))
	{
		OnReceiveFieldsOverlap(OtherFieldTags, OtherField);
		//OnFieldsOverlap.Broadcast(OtherFieldTags, OtherField);
	}
	OnBeginFieldOverlap.Broadcast(OtherFieldTags, OtherActor, bFromSweep, SweepResult);

}
/*
	Need to think of good way of handling cases, which can accept more than one tag.
	Ie. Fire field can have at least two interactions:
	1. When affected by water field it intensity will be decreased or even extinguished entirely.
	2. When affected by oil it's intensity will increase for short amount of time.

	Field type is identified by tag. Coz I want for everyone to be able to make any tags
	and just assign them to fields.

	The problem is I can't reliably predict what tags will be defined.

	So we end up with two solutions:
	1. Hardcoded list of possible tags and triggering right function depending on tag.
	2. Let the users do whatever and defined approperiate behavior in blueprint, leave the
	C++, just for checking tags, firing timers, destroying, replication, and transition handling from
	one field to another.

	//UPDATE:

	Just make rigid setup. Field X can only respond to Y and Z or just to B.
	Use attributes to handle fields with well attributes (Health or something).

	Respond behaviors should be created in blueprint.
	C++ should just encapsulate common needed functionality and simplify process by reducing
	amount of nodes you need to place in blueprint ;).
	*/

void AARFieldBase::OnCombineFields_Implementation(TSubclassOf<class AARFieldBase> NewField, float DurationIn, float TickIntervalIn)
{
	if (!NewField || NewField == this->GetClass())
		return;
	FTransform SpawnTM(FRotator(0, 0, 0), GetActorLocation());
	AARFieldBase* field = Cast<AARFieldBase>(UGameplayStatics::BeginSpawningActorFromClass(this, NewField, SpawnTM, true));
	if (field)
	{
		field->MaximumLifeTime = DurationIn;
		field->TickInterval = TickIntervalIn;
		field->SetOwner(GetOwner());
		field->InstigatorController = InstigatorController;
		field->Initialize();
		UGameplayStatics::FinishSpawningActor(field, SpawnTM);
		return;
	}
}

void AARFieldBase::SetOnFieldsOverlap(FFieldTags OtherFieldTagsIn, class AARFieldBase* OtherField)
{
	//two fields are overlapping
	//check if they have compatibile tags.
	if (OtherFieldTagsIn.OwnedTags.MatchesAny(RequiredTags, false))
	{
		this->Deactivate();
		OtherField->Deactivate();
		OnCombineFields(NULL, 0, 0);

		//we got this far. We can assume that combo is successful.
		OtherField->Died();
		this->Died();
	}
}
void AARFieldBase::ClearTimers()
{
	//clear timers. We don't want actor to tick or something at this point.
	GetWorld()->GetTimerManager().ClearTimer(LifeHandle);
	GetWorld()->GetTimerManager().ClearTimer(IntervalHandle);
}
void AARFieldBase::Deactivate()
{
	Collision->Deactivate();
}
void AARFieldBase::SelfClean()
{
	//last chance to exectute some logic in blueprint, before actor is destroyed
	OnFieldBeginDestroy();

	//hide actor
	bHidden = true;
	//destroy after 3 seconds;
	SetLifeSpan(3);
}

void AARFieldBase::SetFieldTick()
{
	OnFieldTick();
	//CharactersInField.Reset();
	//TArray<FOverlapResult> OverlapResult;
	//FCollisionObjectQueryParams ObjectQuery;
	//FCollisionQueryParams CollisionParams;
	//CollisionParams.AddIgnoredActor(this);
	//bool success = GetWorld()->OverlapMulti(OverlapResult, this->GetActorLocation(), FQuat::Identity, Collision->GetCollisionShape(), CollisionParams, ObjectQuery);

	////ObjectQuery.
	////Collision->ComponentOverlapMulti(OverlapResult, GetWorld(), this->GetActorLocation(), FRotator(0,0,0), ECollisionChannel::ECC_Pawn, FComponentQueryParams::FComponentQueryParams(), )

	//if (success)
	//{
	//	for (FOverlapResult& result : OverlapResult)
	//	{
	//		if (result.Actor.IsValid())
	//		{
	//			CharactersInField.AddUnique(Cast<AARCharacter>(result.Actor.Get()));
	//		}
	//	}
	//}
}