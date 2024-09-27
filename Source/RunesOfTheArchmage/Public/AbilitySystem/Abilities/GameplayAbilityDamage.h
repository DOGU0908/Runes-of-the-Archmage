// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "AbilityTypes.h"
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
	
	FDamageEffectParams MakeDamageEffectParams(AActor* TargetActor = nullptr) const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DebuffChance = 25.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DebuffDamage = 5.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DebuffFrequency = 1.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DebuffDuration = 4.f;
	
};
