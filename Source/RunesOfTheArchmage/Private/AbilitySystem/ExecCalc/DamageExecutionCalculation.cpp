// Copyright DOGU0908


#include "AbilitySystem/ExecCalc/DamageExecutionCalculation.h"

#include "AbilitySystemComponent.h"
#include "AbilityTypes.h"
#include "GameplayTagSingleton.h"
#include "AbilitySystem/CharacterAbilitySystemComponent.h"
#include "AbilitySystem/CharacterAttributeSet.h"

struct FDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalChance);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(IceResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ThunderResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

	DECLARE_ATTRIBUTE_CAPTUREDEF(FireBonus);
	DECLARE_ATTRIBUTE_CAPTUREDEF(IceBonus);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ThunderBonus);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalBonus);
	
	FDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, CriticalChance, Source, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, IceResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, ThunderResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, PhysicalResistance, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, FireBonus, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, IceBonus, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, ThunderBonus, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, PhysicalBonus, Source, false);
	}
};

static const FDamageStatics& GetDamageStatics()
{
	static FDamageStatics DamageStaticsObject;
	
	return DamageStaticsObject;
}

UDamageExecutionCalculation::UDamageExecutionCalculation()
{
	RelevantAttributesToCapture.Add(GetDamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().CriticalChanceDef);
	
	RelevantAttributesToCapture.Add(GetDamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().IceResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().ThunderResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().PhysicalResistanceDef);

	RelevantAttributesToCapture.Add(GetDamageStatics().FireBonusDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().IceBonusDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().ThunderBonusDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().PhysicalBonusDef);
}

void UDamageExecutionCalculation::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	const FGameplayTagSingleton& GameplayTags = FGameplayTagSingleton::Get();
	UCharacterAbilitySystemComponent* SourceAbilitySystemComponent = Cast<UCharacterAbilitySystemComponent>(ExecutionParams.GetSourceAbilitySystemComponent());
	
	TagsToCaptureDefs.Add(GameplayTags.AttributesSecondaryArmor, GetDamageStatics().ArmorDef);
	TagsToCaptureDefs.Add(GameplayTags.AttributesSecondaryCriticalChance, GetDamageStatics().CriticalChanceDef);

	TagsToCaptureDefs.Add(GameplayTags.AttributesResistancesFire, GetDamageStatics().FireResistanceDef);
	TagsToCaptureDefs.Add(GameplayTags.AttributesResistancesIce, GetDamageStatics().IceResistanceDef);
	TagsToCaptureDefs.Add(GameplayTags.AttributesResistancesThunder, GetDamageStatics().ThunderResistanceDef);
	TagsToCaptureDefs.Add(GameplayTags.AttributesResistancesPhysical, GetDamageStatics().PhysicalResistanceDef);

	TagsToCaptureDefs.Add(GameplayTags.AttributesBonusesFire, GetDamageStatics().FireBonusDef);
	TagsToCaptureDefs.Add(GameplayTags.AttributesBonusesIce, GetDamageStatics().IceBonusDef);
	TagsToCaptureDefs.Add(GameplayTags.AttributesBonusesThunder, GetDamageStatics().ThunderBonusDef);
	TagsToCaptureDefs.Add(GameplayTags.AttributesBonusesPhysical, GetDamageStatics().PhysicalBonusDef);
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextBase* GameplayEffectContext = static_cast<FGameplayEffectContextBase*>(Spec.GetContext().Get());

	FAggregatorEvaluateParameters AggregatorEvaluateParameters;
	AggregatorEvaluateParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	AggregatorEvaluateParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// check debuff
	for (const auto& Pair: GameplayTags.DamageTypesToDebuffs)
	{
		const FGameplayTag& DamageType = Pair.Key;
		
		// -1.f is no damage, not healing
		if (Spec.GetSetByCallerMagnitude(DamageType, false, -1.f) >= 0.f)
		{
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(GameplayTags.DebuffPropertyChance, false, -1.f);

			float TargetDebuffResistance = 0.f;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(TagsToCaptureDefs[GameplayTags.DamageTypesToResistances[DamageType]], AggregatorEvaluateParameters, TargetDebuffResistance);
			TargetDebuffResistance = FMath::Max<float>(TargetDebuffResistance, 0.f);
			const float EffectiveDebuffChance = SourceDebuffChance * (1.f - TargetDebuffResistance / 100.f);

			if (FMath::RandRange(1, 100) < EffectiveDebuffChance)
			{
				GameplayEffectContext->SetIsSuccessfulDebuff(true);
				GameplayEffectContext->SetDamageType(DamageType);

				const float DebuffDamage = Spec.GetSetByCallerMagnitude(GameplayTags.DebuffPropertyDamage, false, -1.f);
				const float DebuffDuration = Spec.GetSetByCallerMagnitude(GameplayTags.DebuffPropertyDuration, false, -1.f);
				const float DebuffFrequency = Spec.GetSetByCallerMagnitude(GameplayTags.DebuffPropertyFrequency, false, -1.f);

				GameplayEffectContext->SetDebuffDamage(DebuffDamage);
				GameplayEffectContext->SetDebuffDuration(DebuffDuration);
				GameplayEffectContext->SetDebuffFrequency(DebuffFrequency);
			}
		}
	}

	float Damage = 0.f;
	for (const auto& Pair: GameplayTags.DamageTypesToResistances)
	{
		checkf(TagsToCaptureDefs.Contains(Pair.Value), TEXT("TagsToCaptureDefs does not contain the tag"));

		// do not show error log for some damage types not existing, this is legit
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key, false);

		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(TagsToCaptureDefs[Pair.Value], AggregatorEvaluateParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);
		
		if (DamageTypeValue > 0.f)
		{
			float DamageTypeBonus = 0.f;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(TagsToCaptureDefs[GameplayTags.DamageTypesToDamageBonuses[Pair.Key]], AggregatorEvaluateParameters, DamageTypeBonus);
			DamageTypeValue *= 1.f + DamageTypeBonus / 10.f;
		}
		
		Damage += DamageTypeValue * (1.f - Resistance / 100.f);
	}

	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ArmorDef, AggregatorEvaluateParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(0.f, TargetArmor);

	Damage = FMath::Max<float>(Damage - TargetArmor * 0.1f, 0.f);

	float SourceCriticalChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().CriticalChanceDef, AggregatorEvaluateParameters, SourceCriticalChance);
	SourceCriticalChance = FMath::Max<float>(0.f, SourceCriticalChance);

	const bool bCriticalHit = FMath::RandRange(1, 100) < SourceCriticalChance;
	GameplayEffectContext->SetIsCriticalHit(bCriticalHit);
	Damage *= bCriticalHit ? 3.f : 1.f;

	const FGameplayModifierEvaluatedData EvaluatedData(UCharacterAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
