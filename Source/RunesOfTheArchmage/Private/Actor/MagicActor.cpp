// Copyright DOGU0908


#include "Actor/MagicActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AMagicActor::AMagicActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AMagicActor::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeSpan);
	SetReplicateMovement(true);
}

bool AMagicActor::ShouldCauseDamage(const AActor* OtherActor) const
{
	if (!DamageEffectParams.SourceAbilitySystemComponent)
	{
		return false;
	}
	
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	// prevent the shooter to generate sphere overlap event
	// if the enemy also has a projectile ability, this is important
	if (SourceAvatarActor == OtherActor)
	{
		return false;
	}

	// prevent friendly unit attack by projectile
	if (!UAbilitySystemLibrary::IsNotFriendlyUnit(SourceAvatarActor, OtherActor))
	{
		return false;
	}

	return true;
}

void AMagicActor::ApplyDamage(AActor* OtherActor)
{
	if (UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		// set target ability system component here (when projectile hits)
		DamageEffectParams.TargetAbilitySystemComponent = TargetAbilitySystemComponent;
		UAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
	}
}

void AMagicActor::Multicast_ShowHitEffect_Implementation(const FVector& SpawnLocation, const FRotator& SpawnRotation) const
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, SpawnLocation, FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, SpawnLocation, SpawnRotation);
}
