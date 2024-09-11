// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/GameplayAbilityBase.h"
#include "GameplayAbilityDamage.generated.h"

/**
 * 
 */
UCLASS()
class RUNESOFTHEARCHMAGE_API UGameplayAbilityDamage : public UGameplayAbilityBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
	
};
