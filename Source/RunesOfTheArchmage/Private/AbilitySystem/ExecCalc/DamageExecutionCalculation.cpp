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
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(IceResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ThunderResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
	
	FDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, CriticalChance, Source, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, IceResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, ThunderResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, PhysicalResistance, Target, false);
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
}

void UDamageExecutionCalculation::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	const FGameplayTagSingleton& Tags = FGameplayTagSingleton::Get();
		
	TagsToCaptureDefs.Add(Tags.AttributesSecondaryArmor, GetDamageStatics().ArmorDef);
	TagsToCaptureDefs.Add(Tags.AttributesSecondaryCriticalChance, GetDamageStatics().CriticalChanceDef);

	TagsToCaptureDefs.Add(Tags.AttributesResistancesFire, GetDamageStatics().FireResistanceDef);
	TagsToCaptureDefs.Add(Tags.AttributesResistancesIce, GetDamageStatics().IceResistanceDef);
	TagsToCaptureDefs.Add(Tags.AttributesResistancesThunder, GetDamageStatics().ThunderResistanceDef);
	TagsToCaptureDefs.Add(Tags.AttributesResistancesPhysical, GetDamageStatics().PhysicalResistanceDef);
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextBase* GameplayEffectContext = static_cast<FGameplayEffectContextBase*>(Spec.GetContext().Get());

	FAggregatorEvaluateParameters AggregatorEvaluateParameters;
	AggregatorEvaluateParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	AggregatorEvaluateParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float Damage = 0.f;
	for (const auto& Pair: FGameplayTagSingleton::Get().DamageTypesToResistances)
	{
		checkf(TagsToCaptureDefs.Contains(Pair.Value), TEXT("TagsToCaptureDefs does not contain the tag"));

		// do not show error log for some damage types not existing, this is legit
		const float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key, false);

		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(TagsToCaptureDefs[Pair.Value], AggregatorEvaluateParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);
		
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
