// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemGlobalsBase.generated.h"

/**
 * 
 */
UCLASS()
class RUNESOFTHEARCHMAGE_API UAbilitySystemGlobalsBase : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	
};
