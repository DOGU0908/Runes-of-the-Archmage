// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMCMaxMana.generated.h"

/**
 * 
 */
UCLASS()
class RUNESOFTHEARCHMAGE_API UMMCMaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMCMaxMana();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition MagicDefinition;
	
};
