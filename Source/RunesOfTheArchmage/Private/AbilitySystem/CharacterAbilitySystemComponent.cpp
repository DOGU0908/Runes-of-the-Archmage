// Copyright DOGU0908


#include "AbilitySystem/CharacterAbilitySystemComponent.h"

#include "AbilitySystem/Abilities/GameplayAbilityBase.h"

void UCharacterAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UCharacterAbilitySystemComponent::OnEffectApplied);
}

void UCharacterAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{
	for (const auto& AbilityClass: Abilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);

		if (const UGameplayAbilityBase* GameplayAbility = Cast<UGameplayAbilityBase>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(GameplayAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}

	// only in server
	bGivenStartupAbilities = true;
	AbilitiesGivenDelegate.Broadcast(this);
}

void UCharacterAbilitySystemComponent::AddCharacterPassiveAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& PassiveAbilities)
{
	for (const auto& AbilityClass: PassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UCharacterAbilitySystemComponent::AbilityPressedByInputTag(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	for (auto& ActivatableAbilitySpec: GetActivatableAbilities())
	{
		if (ActivatableAbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			// TODO: do certain tasks?
		}
	}
}

void UCharacterAbilitySystemComponent::AbilityHeldByInputTag(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	for (auto& ActivatableAbilitySpec: GetActivatableAbilities())
	{
		if (ActivatableAbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(ActivatableAbilitySpec);
			if (!ActivatableAbilitySpec.IsActive())
			{
				TryActivateAbility(ActivatableAbilitySpec.Handle);
			}
		}
	}
}

void UCharacterAbilitySystemComponent::AbilityReleasedByInputTag(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	for (auto& ActivatableAbilitySpec: GetActivatableAbilities())
	{
		if (ActivatableAbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(ActivatableAbilitySpec);
		}
	}
}

void UCharacterAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	// lock change in activatable abilities until this scope is end
	FScopedAbilityListLock ActiveScopeLock(*this);
	
	for (const auto& AbilitySpec: GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to execute delegate for the ability"));
		}
	}
}

FGameplayTag UCharacterAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (auto Tag: AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}

	return FGameplayTag();
}

FGameplayTag UCharacterAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (auto Tag: AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}

	return FGameplayTag();
}

void UCharacterAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	// client side ability icon set through replicating the ability activate
	if (!bGivenStartupAbilities)
	{
		bGivenStartupAbilities = true;
		AbilitiesGivenDelegate.Broadcast(this);
	}
}

void UCharacterAbilitySystemComponent::OnEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                      const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle) const
{
	FGameplayTagContainer GameplayTagContainer;
	EffectSpec.GetAllAssetTags(GameplayTagContainer);

	EffectAssetTags.Broadcast(GameplayTagContainer);
}
