// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/GameplayAbilityDamage.h"
#include "ProjectileGameplayAbility.generated.h"

class AProjectile;
/**
 * 
 */
UCLASS()
class RUNESOFTHEARCHMAGE_API UProjectileGameplayAbility : public UGameplayAbilityDamage
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AProjectile> ProjectileClass;

	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(const FVector& TargetLocation);
	
};
