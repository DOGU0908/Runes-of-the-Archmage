// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/GameplayAbilityBase.h"
#include "PassiveGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class RUNESOFTHEARCHMAGE_API UPassiveGameplayAbility : public UGameplayAbilityBase
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	void OnDeactivate(const FGameplayTag& AbilityTag);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> PassiveEffectClass;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PassiveAttributeTag;

	UPROPERTY(EditDefaultsOnly)
	float PassiveEffectMultiplier = 1.5f;

private:
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle;

	void OnPassiveAbilityUpgrade(const FGameplayTag& AbilityTag);

	void ApplyPassiveEffect();

	void RemovePassiveEffect();
};
