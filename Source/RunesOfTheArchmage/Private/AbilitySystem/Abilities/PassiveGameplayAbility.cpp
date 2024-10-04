// Copyright DOGU0908


#include "AbilitySystem/Abilities/PassiveGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/CharacterAbilitySystemComponent.h"

void UPassiveGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UCharacterAbilitySystemComponent* CharacterAbilitySystemComponent = Cast<UCharacterAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		CharacterAbilitySystemComponent->DeactivatePassiveAbility.RemoveAll(this);
		CharacterAbilitySystemComponent->OnPassiveAbilityUpgradeDelegate.RemoveAll(this);
		CharacterAbilitySystemComponent->DeactivatePassiveAbility.AddUObject(this, &UPassiveGameplayAbility::OnDeactivate);
		CharacterAbilitySystemComponent->OnPassiveAbilityUpgradeDelegate.AddUObject(this, &UPassiveGameplayAbility::OnPassiveAbilityUpgrade);

		ApplyPassiveEffect();
	}
}

void UPassiveGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	RemovePassiveEffect();
}

void UPassiveGameplayAbility::OnDeactivate(const FGameplayTag& AbilityTag)
{
	if (AbilityTags.HasTagExact(AbilityTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

void UPassiveGameplayAbility::OnPassiveAbilityUpgrade(const FGameplayTag& AbilityTag)
{
	if (AbilityTags.HasTagExact(AbilityTag))
	{
		RemovePassiveEffect();
		
		ApplyPassiveEffect();
	}
}

void UPassiveGameplayAbility::ApplyPassiveEffect()
{
	if (UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo())
	{
		const FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
		const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(PassiveEffectClass, GetAbilityLevel(), EffectContextHandle);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, PassiveAttributeTag, PassiveEffectMultiplier * GetAbilityLevel());
		ActiveGameplayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void UPassiveGameplayAbility::RemovePassiveEffect()
{
	if (UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo())
	{
		if (ActiveGameplayEffectHandle.IsValid())
		{
			AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveGameplayEffectHandle);
			ActiveGameplayEffectHandle = FActiveGameplayEffectHandle();
		}
	}
}
