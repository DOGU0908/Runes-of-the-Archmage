// Copyright DOGU0908


#include "AbilitySystem/ExecCalc/DamageExecutionCalculation.h"

#include "AbilitySystemComponent.h"
#include "AbilityTypes.h"
#include "GameplayTagSingleton.h"
#include "AbilitySystem/CharacterAttributeSet.h"

struct FDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalChance);
	
	FDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, CriticalChance, Source, false);
	}
};

static const FDamageStatics& DamageStatics()
{
	static FDamageStatics DamageStaticsObject;
	
	return DamageStaticsObject;
}

UDamageExecutionCalculation::UDamageExecutionCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalChanceDef);
}

void UDamageExecutionCalculation::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextBase* GameplayEffectContext = static_cast<FGameplayEffectContextBase*>(Spec.GetContext().Get());

	FAggregatorEvaluateParameters AggregatorEvaluateParameters;
	AggregatorEvaluateParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	AggregatorEvaluateParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float Damage = Spec.GetSetByCallerMagnitude(FGameplayTagSingleton::Get().Damage);

	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, AggregatorEvaluateParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(0.f, TargetArmor);

	Damage = FMath::Max<float>(Damage - TargetArmor * 0.1f, 0.f);

	float SourceCriticalChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalChanceDef, AggregatorEvaluateParameters, SourceCriticalChance);
	SourceCriticalChance = FMath::Max<float>(0.f, SourceCriticalChance);

	const bool bCriticalHit = FMath::RandRange(1, 100) < SourceCriticalChance;
	GameplayEffectContext->SetIsCriticalHit(bCriticalHit);
	Damage *= bCriticalHit ? 3.f : 1.f;

	const FGameplayModifierEvaluatedData EvaluatedData(UCharacterAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
