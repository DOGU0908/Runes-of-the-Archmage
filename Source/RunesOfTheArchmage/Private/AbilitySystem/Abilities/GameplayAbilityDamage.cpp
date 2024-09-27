// Copyright DOGU0908


#include "AbilitySystem/Abilities/GameplayAbilityDamage.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AbilitySystemLibrary.h"

void UGameplayAbilityDamage::CauseDamage(AActor* TargetActor)
{
	UAbilitySystemLibrary::ApplyDamageEffect(MakeDamageEffectParams(TargetActor));
}

FDamageEffectParams UGameplayAbilityDamage::MakeDamageEffectParams(AActor* TargetActor) const
{
	FDamageEffectParams DamageEffectParams;

	DamageEffectParams.WorldObject = GetAvatarActorFromActorInfo();
	DamageEffectParams.DamageGameplayEffectClass = DamageEffectClass;
	DamageEffectParams.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	DamageEffectParams.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	DamageEffectParams.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	DamageEffectParams.AbilityLevel = GetAbilityLevel();
	DamageEffectParams.DamageType = DamageType;
	DamageEffectParams.DebuffChance = DebuffChance;
	DamageEffectParams.DebuffDamage = DebuffDamage;
	DamageEffectParams.DebuffDuration = DebuffDuration;
	DamageEffectParams.DebuffFrequency = DebuffFrequency;

	return DamageEffectParams;
}
