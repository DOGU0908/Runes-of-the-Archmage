// Copyright DOGU0908


#include "AbilitySystem/CharacterAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagSingleton.h"
#include "AbilitySystem/AbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/GameplayAbilityBase.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Character/PlayerInterface.h"

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
			AbilitySpec.DynamicAbilityTags.AddTag(FGameplayTagSingleton::Get().Abilities_Status_Equipped);
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

FGameplayTag UCharacterAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag: AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return Tag;
		}
	}

	return FGameplayTag();
}

void UCharacterAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
		{
			// send info to server to upgrade attribute
			ServerUpgradeAttribute(AttributeTag);
		}
	}
}

FGameplayAbilitySpec* UCharacterAbilitySystemComponent::GetSpecByAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLoc(*this);

	for (FGameplayAbilitySpec& AbilitySpec: GetActivatableAbilities())
	{
		for (FGameplayTag Tag: AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}

	return nullptr;
}

void UCharacterAbilitySystemComponent::UpdateAbilityStatus(int32 Level)
{
	for (const auto& Info: UAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor())->AbilityInfo)
	{
		if (!Info.AbilityTag.IsValid())
		{
			continue;
		}
		if (Level < Info.LevelRequirement)
		{
			continue;
		}
		
		if (!GetSpecByAbilityTag(Info.AbilityTag))
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.AbilityClass, 1);
			AbilitySpec.DynamicAbilityTags.AddTag(FGameplayTagSingleton::Get().Abilities_Status_Eligible);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);
			ClientUpdateAbilityStatus(Info.AbilityTag, FGameplayTagSingleton::Get().Abilities_Status_Eligible, 1);
		}
	}
}

void UCharacterAbilitySystemComponent::ServerSpendSpellPoint_Implementation(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecByAbilityTag(AbilityTag))
	{
		if (GetAvatarActor()->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddSpellPoints(GetAvatarActor(), -1);
		}
		
		// this is not locked ability because spec exists
		FGameplayTag Status = GetStatusFromSpec(*AbilitySpec);

		const FGameplayTagSingleton GameplayTags = FGameplayTagSingleton::Get();

		if (Status.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
		{
			AbilitySpec->DynamicAbilityTags.RemoveTag(GameplayTags.Abilities_Status_Eligible);
			AbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.Abilities_Status_Unlocked);
			Status = GameplayTags.Abilities_Status_Unlocked;
		}
		else if (Status.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked) || Status.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
		{
			AbilitySpec->Level += 1;
		}

		ClientUpdateAbilityStatus(AbilityTag, Status, AbilitySpec->Level);
		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

void UCharacterAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.0f;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);

	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddAttributePoints(GetAvatarActor(), -1);
	}
}

bool UCharacterAbilitySystemComponent::GetInfoByAbilityTag(const FGameplayTag& AbilityTag, FString& Name, int32& Level)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecByAbilityTag(AbilityTag))
	{
		if (const UGameplayAbilityBase* Ability = Cast<UGameplayAbilityBase>(AbilitySpec->Ability))
		{
			Name = Ability->GetName();
			if (GetStatusFromSpec(*AbilitySpec).MatchesTagExact(FGameplayTagSingleton::Get().Abilities_Status_Eligible))
			{
				Level = 0;
			}
			else
			{
				Level = AbilitySpec->Level;
			}
			return true;
		}
	}

	Name = FString();
	Level = 0;
	return false;
}

void UCharacterAbilitySystemComponent::ClientEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag,
	const FGameplayTag& InputTag, const FGameplayTag& PrevInputTag)
{
	OnAbilityEquipped.Broadcast(AbilityTag, StatusTag, InputTag, PrevInputTag);
}

void UCharacterAbilitySystemComponent::ClearInputSlot(FGameplayAbilitySpec* Spec)
{
	const FGameplayTag InputTag = GetInputTagFromSpec(*Spec);
	Spec->DynamicAbilityTags.RemoveTag(InputTag);
	MarkAbilitySpecDirty(*Spec);
}

void UCharacterAbilitySystemComponent::ClearAbilitiesOfInputSlot(const FGameplayTag& InputTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& Spec: GetActivatableAbilities())
	{
		if (AbilityHasInputSlot(&Spec, InputTag))
		{
			ClearInputSlot(&Spec);
		}
	}
}

bool UCharacterAbilitySystemComponent::AbilityHasInputSlot(const FGameplayAbilitySpec* Spec,
	const FGameplayTag& InputTag)
{
	for (FGameplayTag Tag: Spec->DynamicAbilityTags)
	{
		if (Tag.MatchesTagExact(InputTag))
		{
			return true;
		}
	}

	return false;
}

void UCharacterAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag,
                                                                         const FGameplayTag& InputTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecByAbilityTag(AbilityTag))
	{
		const FGameplayTagSingleton GameplayTags = FGameplayTagSingleton::Get();
		
		const FGameplayTag& PrevInputTag = GetInputTagFromSpec(*AbilitySpec);
		
		const FGameplayTag& StatusTag = GetStatusFromSpec(*AbilitySpec);
		if (StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Equipped) || StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
		{
			// clear other abilities of this input slot
			ClearAbilitiesOfInputSlot(InputTag);

			// clear this ability input slot
			ClearInputSlot(AbilitySpec);
			AbilitySpec->DynamicAbilityTags.AddTag(InputTag);

			// set the ability to be equipped
			if (StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
			{
				AbilitySpec->DynamicAbilityTags.RemoveTag(GameplayTags.Abilities_Status_Unlocked);
				AbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.Abilities_Status_Equipped);
			}
			MarkAbilitySpecDirty(*AbilitySpec);
		}
		
		ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, InputTag, PrevInputTag);
	}
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

void UCharacterAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag, int32 AbilityLevel)
{
	OnAbilityStatusChanged.Broadcast(AbilityTag, StatusTag, AbilityLevel);
}

void UCharacterAbilitySystemComponent::OnEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                      const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle) const
{
	FGameplayTagContainer GameplayTagContainer;
	EffectSpec.GetAllAssetTags(GameplayTagContainer);

	EffectAssetTags.Broadcast(GameplayTagContainer);
}
