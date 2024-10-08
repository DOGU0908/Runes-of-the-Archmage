// Copyright DOGU0908


#include "Actor/EffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AEffectActor::AEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
}

void AEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	// apply effect to target may be called directly on the blueprint, so check effect application on target must also be called here
	if (!CheckEffectApplicationOnTarget(TargetActor))
	{
		return;
	}
	
	UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetAbilitySystemComponent)
		return;
	
	// apply effect
	check(GameplayEffectClass);
	// gameplay effect context holds information such as the source of the effect, target, and hit results
	FGameplayEffectContextHandle EffectContextHandle = TargetAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	// gameplay effect spec includes information about the effect, such as magnitude, duration and any other attribute modifiers or gameplay tags
	// level parameter is used to scale the magnitude of any gameplay effect modifiers, it can be used to set the value of the curve horizontal axis
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetAbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveGameplayEffectHandle = TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	if (EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite)
	{
		if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::DoNotRemove)
			return;
		
		ActiveEffectHandles.Add(ActiveGameplayEffectHandle, TargetAbilitySystemComponent);
	}
	else if (bDestroyOnEffectApplication)
	{
		Destroy();
	}
}

void AEffectActor::OnOverlap(AActor* TargetActor)
{
	if (!CheckEffectApplicationOnTarget(TargetActor))
	{
		return;
	}
	
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (!CheckEffectApplicationOnTarget(TargetActor))
	{
		return;
	}
	
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}

	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetAbilitySystemComponent))
			return;

		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		
		for (auto& HandlePair: ActiveEffectHandles)
		{
			if (TargetAbilitySystemComponent == HandlePair.Value)
			{
				TargetAbilitySystemComponent->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}

		for (auto& Handle: HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}

// do not implement this performance heavy
bool AEffectActor::CheckEffectApplicationOnTarget(const AActor* TargetActor) const
{
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies)
	{
		return false;
	}

	// can add when creating an effect that not applies to players - not in this project

	return true;
}
