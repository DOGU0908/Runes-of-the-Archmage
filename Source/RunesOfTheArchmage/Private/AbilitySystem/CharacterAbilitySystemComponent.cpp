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

void UCharacterAbilitySystemComponent::OnEffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                       const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle) const
{
	FGameplayTagContainer GameplayTagContainer;
	EffectSpec.GetAllAssetTags(GameplayTagContainer);

	EffectAssetTags.Broadcast(GameplayTagContainer);
}
