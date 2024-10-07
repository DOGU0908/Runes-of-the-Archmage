// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/GameplayAbilityDamage.h"
#include "SummonGameplayAbility.generated.h"

class AStorm;
/**
 * 
 */
UCLASS()
class RUNESOFTHEARCHMAGE_API USummonGameplayAbility : public UGameplayAbilityDamage
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AStorm> StormClass;

	UFUNCTION(BlueprintCallable)
	void SpawnStorm(const FVector& TargetLocation);
	
};
