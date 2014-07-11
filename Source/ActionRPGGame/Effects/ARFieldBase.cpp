// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "ARFieldBase.h"

AARFieldBase::AARFieldBase(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	//it might be better to use TimerManager instead. We will see.


	Collision = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("Collision"));
	RootComponent = Collision;
	//Mesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));

	//FScriptDelegate overlapDel;
	//overlapDel.BindUFunction(this, &AARFieldBase::SetOnBeginOverlap);

	//Collision->OnComponentBeginOverlap.Add(overlapDel);

	Collision->OnComponentBeginOverlap.AddDynamic(this, &AARFieldBase::SetOnBeginOverlap);

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
	OnFieldInitialized();
	if (!IsLifeInfinite)
	{
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();

		FTimerDelegate Del = FTimerDelegate::CreateUObject(this, &AARFieldBase::SelfClean);
		TimerManager.SetTimer(LifeHandle, Del, MaximumLifeTime, false, MaximumLifeTime);
	}

	if (IsTicking)
	{
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		FTimerDelegate TickDelegate = FTimerDelegate::CreateUObject(this, &AARFieldBase::SetFieldTick);
		TimerManager.SetTimer(IntervalHandle, TickDelegate, TickInterval, true);
	}
}

void AARFieldBase::SpawnField(TSubclassOf<class AARFieldBase> Field, float Duration, float TickIntervalIn, FVector Location, AARFieldBase* OtherField)
{
	if (!Field)
		return;

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
	this->Destroy();
	OtherField->Destroy();
}

void AARFieldBase::SetOnBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
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
	OnBeginFieldOverlap.Broadcast(OtherFieldTags, OtherActor, OtherComp, bFromSweep, SweepResult);
}

void AARFieldBase::SelfClean()
{
	OnFieldBeginDestroy();
	this->Destroy();
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