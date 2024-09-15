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

public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
	
};
