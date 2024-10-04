// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CharacterAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&);
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilitiesGiven, UCharacterAbilitySystemComponent*);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel);
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquipped, const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& InputTag, const FGameplayTag& PrevInputTag);
DECLARE_MULTICAST_DELEGATE_OneParam(FDeactivatePassiveAbility, const FGameplayTag& AbilityTag);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPassiveAbilityUpgrade, const FGameplayTag& AbilityTag);

/**
 * 
 */
UCLASS()
class RUNESOFTHEARCHMAGE_API UCharacterAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& PassiveAbilities);

	void AbilityPressedByInputTag(const FGameplayTag& InputTag);
	void AbilityHeldByInputTag(const FGameplayTag& InputTag);
	void AbilityReleasedByInputTag(const FGameplayTag& InputTag);

	FAbilitiesGiven AbilitiesGivenDelegate;
	bool bGivenStartupAbilities = false;

	void ForEachAbility(const FForEachAbility& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

	FGameplayAbilitySpec* GetSpecByAbilityTag(const FGameplayTag& AbilityTag);
	void UpdateAbilityStatus(int32 Level);

	FAbilityStatusChanged OnAbilityStatusChanged;

	UFUNCTION(Server, Reliable)
	void ServerSpendSpellPoint(const FGameplayTag& AbilityTag);
	
	bool GetInfoByAbilityTag(const FGameplayTag& AbilityTag, FString& Name, int32& Level);

	UFUNCTION(Server, Reliable)
	void ServerEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag);

	UFUNCTION(Client, Reliable)
	void ClientEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& InputTag, const FGameplayTag& PrevInputTag);

	static void ClearInputSlot(FGameplayAbilitySpec* Spec);
	static bool AbilityHasInputSlot(const FGameplayAbilitySpec& Spec, const FGameplayTag& InputTag);
	
	FAbilityEquipped OnAbilityEquipped;

	FDeactivatePassiveAbility DeactivatePassiveAbility;

	bool IsInputSlotEmpty(const FGameplayTag& InputTag);
	FGameplayAbilitySpec* GetSpecByInputTag(const FGameplayTag& InputTag);
	bool IsPassiveSpell(const FGameplayAbilitySpec& Spec) const;

	FOnPassiveAbilityUpgrade OnPassiveAbilityUpgradeDelegate;
	
protected:
	// replicate to client
	UFUNCTION(Client, Reliable)
	void OnEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle) const;

	virtual void OnRep_ActivateAbilities() override;

	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel);
	
};
