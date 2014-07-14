// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../Effects/AREffectPeriodic.h"
#include "../Effects/AREffectPeriodicO.h"
#include "../Componenets/ARAttributeComponent.h"
#include "../Types/AREffectTypes.h"
#include "ARTraceStatics.h"

#include "../ARProjectile.h"
#include "../Effects/ARFieldBase.h"

#include "AREffectStatics.h"

UAREffectStatics::UAREffectStatics(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}

FEffectSpec UAREffectStatics::CreatePeriodicEffect(AActor* EffectTarget, AActor* EffectCauser, float Duration, TSubclassOf<class AAREffectPeriodic> EffectType, FEffectCue EffectCue, TSubclassOf<class AARActorCue> ActorCue)
{
	FEffectSpec PeriodicEffect;
	if (!EffectTarget && !EffectCauser)
		return PeriodicEffect;
	UARAttributeComponent* attrComp = EffectTarget->FindComponentByClass<UARAttributeComponent>();

	if (attrComp->ActiveEffects.Effects.Num() > 0)
	{
		for (FEffectSpec& effect : attrComp->ActiveEffects.Effects)
		{
			if (effect.ActorEffect && effect.ActorEffect->GetClass() == EffectType)
			{
				//reality. Do check if effect is stackable or something.
				return PeriodicEffect;
			}
		}
	}

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoCollisionFail = true;
	SpawnInfo.Owner = EffectTarget;
	//SpawnInfo.Instigator = EffectCauser;

	AAREffectPeriodic* effecTemp = EffectTarget->GetWorld()->SpawnActor<AAREffectPeriodic>(EffectType, SpawnInfo);
	effecTemp->EffectCauser = EffectCauser;
	effecTemp->EffectTarget = EffectTarget;
	effecTemp->MaxDuration = Duration;
	PeriodicEffect.ActorEffect = effecTemp;
	PeriodicEffect.MaxDuration = Duration;
	PeriodicEffect.CurrentDuration = 0;
	PeriodicEffect.EffectCue = EffectCue;
	PeriodicEffect.ActorCue = ActorCue;
	attrComp->AddPeriodicEffect(PeriodicEffect);

	return PeriodicEffect;
}

void UAREffectStatics::ActivatePeriodicEffect(FPeriodicEffect PeriodicEffect)
{
	//PeriodicEffect.PeriodicEffect->Initialze();
	UARAttributeComponent* attrComp = PeriodicEffect.PeriodicEffect->GetOwner()->FindComponentByClass<UARAttributeComponent>();
	PeriodicEffect.IsEffectActive = true;

	//attrComp->AddPeriodicEffect(PeriodicEffect);
}

void UAREffectStatics::ApplyPeriodicEffect(AActor* EffectTarget, AActor* EffectCauser, float Duration, TSubclassOf<class UAREffectPeriodicO> EffectType)
{
	//if (!EffectCauser && !EffectTarget)
	//	return;

	//UARAttributeBaseComponent* attrTarget = EffectTarget->FindComponentByClass<UARAttributeBaseComponent>();

	//UAREffectPeriodicO* effect = ConstructObject<UAREffectPeriodicO>(EffectType);

	//effect->EffectCausedBy = EffectCauser;
	//effect->EffectTarget = EffectTarget;
	//effect->MaxDuration = Duration;
	//FEffectSpec Spec;
	//Spec.Effect = effect;
	//Spec.MaxDuration = Duration;
	//Spec.CurrentDuration = 0;
	////attrTarget->ActiveEffects.Effects.Add(Spec);
	//attrTarget->ApplyPeriodicEffect(Spec);
}

void UAREffectStatics::ChangeAttribute(AActor* Target, AActor* CausedBy, float ModVal, FName AttributeName, TEnumAsByte<EAttrOp> OpType)
{
	if (!Target && !CausedBy)
		return;

	TWeakObjectPtr<UARAttributeBaseComponent> attrComp = Target->FindComponentByClass<UARAttributeBaseComponent>();

	if (!attrComp.IsValid())
		return;
	attrComp->ChangeAttribute(AttributeName, ModVal, OpType);
}

void UAREffectStatics::ApplyDamage(AActor* DamageTarget, float BaseDamage, FName AttributeName, AActor* EventInstigator, AActor* DamageCauser, FGameplayTagContainer DamageTag, bool bIsComboFinisher, TSubclassOf<class UDamageType> DamageType)
{
	if (!DamageTarget)
		return;

	TWeakObjectPtr<UARAttributeBaseComponent> attr = DamageTarget->FindComponentByClass<UARAttributeBaseComponent>();

	FAttribute Attribute;
	Attribute.AttributeName = AttributeName;
	Attribute.ModValue = BaseDamage;
	Attribute.OperationType = EAttrOp::Attr_Subtract;

	FARDamageEvent DamageEvent;
	DamageEvent.Attribute = Attribute;
	DamageEvent.DamageTypeClass = DamageType;
	DamageEvent.DamageTag = DamageTag;
	DamageEvent.IsComboFinisher = bIsComboFinisher;
	attr->DamageAttribute(DamageEvent, EventInstigator, DamageCauser);
}

void UAREffectStatics::ApplyPointDamage(AActor* DamageTarget, float AttributeMod, FName AttributeName, const FVector& HitFromLocation, const FHitResult& HitInfo, AActor* EventInstigator, AActor* Causer, bool bIsComboFinisher, TSubclassOf<class UDamageType> DamageType)
{
	if (!DamageTarget)
		return;

	TWeakObjectPtr<UARAttributeBaseComponent> attrComp = DamageTarget->FindComponentByClass<UARAttributeBaseComponent>();

	if (!attrComp.IsValid())
		return;

	FAttribute Attribute;
	Attribute.AttributeName = AttributeName;
	Attribute.ModValue = AttributeMod;
	Attribute.OperationType = EAttrOp::Attr_Subtract;

	//FPointAttributeChangeEvent AttributeEvent(Attribute, HitInfo, HitFromLocation, DamageType);
	FARPointDamageEvent AttributeEvent;
	AttributeEvent.Attribute = Attribute;
	AttributeEvent.HitInfo = HitInfo;
	AttributeEvent.ShotDirection = HitFromLocation;
	AttributeEvent.DamageTypeClass = DamageType;
	AttributeEvent.IsComboFinisher = bIsComboFinisher;
	attrComp->DamageAttribute(AttributeEvent, EventInstigator, Causer);
}

void UAREffectStatics::ApplyRadialDamageWithFalloff(FName AttributeName, float BaseDamage, float MinimumDamage, const FVector& Origin, TEnumAsByte<ECollisionChannel> Collision, float DamageInnerRadius, float DamageOuterRadius, float DamageFalloff, TSubclassOf<class UDamageType> DamageTypeClass, const TArray<AActor*>& IgnoreActors, FGameplayTagContainer DamageTag, bool bIsComboFinisher, AActor* DamageCauser, AActor* Instigator)
{
	if (!DamageCauser)
		return;

	static FName NAME_AttributeRadialDamage = FName(TEXT("ApplyRadialDamage"));
	FCollisionQueryParams SphereParams(NAME_AttributeRadialDamage, false, nullptr);
	SphereParams.AddIgnoredActors(IgnoreActors);
	FCollisionResponseParams ResponseParams;
	//ResponseParams.CollisionResponse.
	
	TArray<FOverlapResult> Overlaps;
	//DamageCauser->GetWorld()->OverlapMulti(Overlaps, Origin, FQuat::Identity, ECollisionChannel::ECC_Pawn, FCollisionShape::MakeSphere(DamageOuterRadius), SphereParams, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects));
	DamageCauser->GetWorld()->OverlapMulti(Overlaps, Origin, FQuat::Identity, Collision, FCollisionShape::MakeSphere(DamageOuterRadius), SphereParams);
	TArray<UARAttributeBaseComponent*> Attributes;

	//create list of attribute component that we can apply damage to.
	for (FOverlapResult& result : Overlaps)
	{
		if (result.Actor.IsValid() && result.Actor->bCanBeDamaged)
		{
			UARAttributeBaseComponent* Attr = result.Actor->FindComponentByClass<UARAttributeBaseComponent>();
			if (Attr)
			{
				Attributes.AddUnique(Attr);
			}
		}
	}
	for (UARAttributeBaseComponent* attr : Attributes)
	{

		FAttribute Attribute;

		Attribute.AttributeName = AttributeName;
		Attribute.ModValue = BaseDamage;
		Attribute.OperationType = EAttrOp::Attr_Subtract;

		FARRadialDamageEvent DamageEvent;

		DamageEvent.Attribute = Attribute;
		DamageEvent.DamageTypeClass = DamageTypeClass;
		DamageEvent.DamageTag = DamageTag;
		DamageEvent.HitInfo.Location = Origin;
		DamageEvent.HitInfo.ImpactPoint = Origin;
		DamageEvent.Radius = DamageOuterRadius;
		DamageEvent.IsComboFinisher = bIsComboFinisher;
		attr->DamageAttribute(DamageEvent, Instigator, DamageCauser);
	}
}

void UAREffectStatics::ApplyRadialDamage(FName AttributeName, float BaseDamage, const FVector& Origin, TEnumAsByte<ECollisionChannel> Collision, float DamageRadius, TSubclassOf<class UDamageType> DamageTypeClass, const TArray<AActor*>& IgnoreActors, AActor* DamageCauser, AActor* Instigator, bool bDoFullDamage, FGameplayTagContainer DamageTag, bool bIsComboFinisher)
{
	ApplyRadialDamageWithFalloff(AttributeName, BaseDamage, BaseDamage, Origin, Collision, DamageRadius, DamageRadius, DamageRadius, DamageTypeClass, IgnoreActors, DamageTag, bIsComboFinisher, DamageCauser, Instigator);
}

void UAREffectStatics::ApplyMultiBoxDamage(FVector StartLocation, float Range, FVector BoxExtends, TEnumAsByte<ECollisionChannel> Collision, AActor* DamageCauser, APawn* DamageInstigator)
{
	if (!DamageCauser || !DamageInstigator)
		return;
	const FVector ShootDir = UARTraceStatics::GetCameraAim(DamageInstigator);
	FVector StartTrace = UARTraceStatics::GetCameraDamageStartLocation(ShootDir, DamageInstigator);
	FVector EndTrace = ((ShootDir * Range) + StartLocation);
	FHitResult EndPoint = UARTraceStatics::RangedTrace(StartLocation, EndTrace, DamageInstigator, EARTraceType::Trace_Weapon);

	TArray<FHitResult> OutHits;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(DamageCauser);

	DamageCauser->GetWorld()->SweepMulti(OutHits, StartLocation, EndTrace, FQuat::Identity, Collision, FCollisionShape::MakeBox(FVector(100, 100, 100)), Params);

	DrawDebugSweptBox(DamageInstigator->GetWorld(), StartLocation, EndTrace, FRotator(0,0,0), FVector(100, 100, 100), FColor::Black, true, 10);
}

void UAREffectStatics::DrawDebugSweptBox(const UWorld* InWorld, FVector const& Start, FVector const& End, FRotator const & Orientation, FVector const & HalfSize, FColor const& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority)
{
	FVector const TraceVec = End - Start;
	float const Dist = TraceVec.Size();

	FVector const Center = Start + TraceVec * 0.5f;

	FQuat const CapsuleRot = Orientation.Quaternion();
	::DrawDebugBox(InWorld, Start, HalfSize, CapsuleRot, Color, bPersistentLines, LifeTime, DepthPriority);

	//now draw lines from vertices
	FVector Vertices[8];
	Vertices[0] = Start + CapsuleRot.RotateVector(FVector(-HalfSize.X, -HalfSize.Y, -HalfSize.Z));	//flt
	Vertices[1] = Start + CapsuleRot.RotateVector(FVector(-HalfSize.X, HalfSize.Y, -HalfSize.Z));	//frt
	Vertices[2] = Start + CapsuleRot.RotateVector(FVector(-HalfSize.X, -HalfSize.Y, HalfSize.Z));	//flb
	Vertices[3] = Start + CapsuleRot.RotateVector(FVector(-HalfSize.X, HalfSize.Y, HalfSize.Z));	//frb
	Vertices[4] = Start + CapsuleRot.RotateVector(FVector(HalfSize.X, -HalfSize.Y, -HalfSize.Z));	//blt
	Vertices[5] = Start + CapsuleRot.RotateVector(FVector(HalfSize.X, HalfSize.Y, -HalfSize.Z));	//brt
	Vertices[6] = Start + CapsuleRot.RotateVector(FVector(HalfSize.X, -HalfSize.Y, HalfSize.Z));	//blb
	Vertices[7] = Start + CapsuleRot.RotateVector(FVector(HalfSize.X, HalfSize.Y, HalfSize.Z));		//brb
	for (int32 VertexIdx = 0; VertexIdx < 8; ++VertexIdx)
	{
		::DrawDebugLine(InWorld, Vertices[VertexIdx], Vertices[VertexIdx] + TraceVec, Color, bPersistentLines, LifeTime, DepthPriority);
	}

	::DrawDebugBox(InWorld, End, HalfSize, CapsuleRot, Color, bPersistentLines, LifeTime, DepthPriority);
}

void UAREffectStatics::ShootProjectile(TSubclassOf<class AARProjectile> Projectile, FVector Origin, FVector ShootDir, AActor* Causer, FName StartSocket, FHitResult HitResult)
{
	APawn* pawn = Cast<APawn>(Causer);
	if (!pawn)
		return;

	if (HitResult.bBlockingHit)
	{
		const FVector dir = (HitResult.ImpactPoint - Origin).SafeNormal();
		FTransform SpawnTM(FRotator(0,0,0), Origin);

		AARProjectile* proj = Cast<AARProjectile>(UGameplayStatics::BeginSpawningActorFromClass(Causer, Projectile, SpawnTM));


		if (proj)
		{
			//proj->Instigator = Causer;
			proj->SetOwner(Causer);
			proj->Movement->Velocity = dir * proj->Movement->InitialSpeed;
			UGameplayStatics::FinishSpawningActor(proj, SpawnTM);
		}
	}

}

void UAREffectStatics::SpawnProjectileInArea(TSubclassOf<class AARProjectile> Projectile, AActor* Causer, APawn* Instigator, FHitResult HitResult, float InitialVelocity, float MaxRadius, float MaxHeight, float ImpactDirection, int32 Amount)
{
	if (HitResult.bBlockingHit)
	{
		for (int32 CurAmount = 0; CurAmount < Amount; CurAmount++)
		{
			FVector Location = HitResult.ImpactPoint;
			Location.Z += MaxHeight;
			Location.Y += FMath::RandRange(-MaxRadius, MaxRadius);
			Location.X += FMath::RandRange(-MaxRadius, MaxRadius);
			FTransform SpawnTM(FRotator(0, 0, 0), Location);

			AARProjectile* proj = Cast<AARProjectile>(UGameplayStatics::BeginSpawningActorFromClass(Causer, Projectile, SpawnTM));

			FVector FallDirection = FVector(FMath::FRandRange(-ImpactDirection, ImpactDirection), FMath::FRandRange(-ImpactDirection, ImpactDirection), -1);

			if (proj)
			{
				//proj->Instigator = Causer;
				proj->SetOwner(Causer);
				proj->Instigator = Instigator;
				proj->Movement->Velocity = FallDirection * InitialVelocity; // proj->Movement->InitialSpeed;
				UGameplayStatics::FinishSpawningActor(proj, SpawnTM);
			}
		}
	}
}

void UAREffectStatics::SpawnField(TSubclassOf<class AARFieldBase> Field, AActor* Instigator, FHitResult Location, float Duration, float TickInterval)
{
	if (!Field && !Instigator)
		return;

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoCollisionFail = true;
	SpawnInfo.Owner = Instigator;
	
	//SpawnInfo.Instigator = Instigator;

	//Instigator->GetWorld()->SpawnActor<AARFieldBase>(Field, Location.Location, FRotator(0, 0, 0), SpawnInfo);

	FTransform SpawnTM(FRotator(0, 0, 0), Location.Location);
	AARFieldBase* field = Cast<AARFieldBase>(UGameplayStatics::BeginSpawningActorFromClass(Instigator, Field, SpawnTM, true));
	if (field)
	{
		field->MaximumLifeTime = Duration;
		field->TickInterval = TickInterval;
		field->SetOwner(Instigator);
		field->Initialize();
		//field->Instigator = Instigator;
		UGameplayStatics::FinishSpawningActor(field, SpawnTM);
	}
}