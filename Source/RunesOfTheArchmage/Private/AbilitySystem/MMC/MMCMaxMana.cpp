// Copyright DOGU0908


#include "AbilitySystem/MMC/MMCMaxMana.h"

#include "AbilitySystem/CharacterAttributeSet.h"
#include "Combat/CombatInterface.h"

UMMCMaxMana::UMMCMaxMana()
{
	MagicDefinition.AttributeToCapture = UCharacterAttributeSet::GetMagicAttribute();
	MagicDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	MagicDefinition.bSnapshot = false;

	RelevantAttributesToCapture.Add(MagicDefinition);
}

float UMMCMaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float Magic = 0.f;
	GetCapturedAttributeMagnitude(MagicDefinition, Spec, EvaluateParameters, Magic);
	Magic = FMath::Max<float>(Magic, 0.f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 Level = CombatInterface->GetCharacterLevel();

	return 30.f * Magic + 20.f * Level;
}
