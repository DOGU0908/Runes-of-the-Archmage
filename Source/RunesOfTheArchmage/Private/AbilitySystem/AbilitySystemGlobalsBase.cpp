// Copyright DOGU0908


#include "AbilitySystem/AbilitySystemGlobalsBase.h"

#include "AbilityTypes.h"

FGameplayEffectContext* UAbilitySystemGlobalsBase::AllocGameplayEffectContext() const
{
	return new FGameplayEffectContextBase();
}
