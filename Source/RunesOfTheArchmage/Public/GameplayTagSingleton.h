// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"

/**
 * 
 */
struct FGameplayTagSingleton
{
public:
	static const FGameplayTagSingleton& Get() {return Instance;}

	static void InitializeNativeGameplayTags();

	// base attributes
	FGameplayTag AttributesBaseHealth;
	FGameplayTag AttributesBaseMana;
	FGameplayTag AttributesBaseStrength;
	FGameplayTag AttributesBaseMagic;
	FGameplayTag AttributesBaseResilience;
	FGameplayTag AttributesBaseVigor;
	
	// secondary attributes
	FGameplayTag AttributesSecondaryArmor;
	FGameplayTag AttributesSecondaryCriticalChance;
	FGameplayTag AttributesSecondaryHealthRegeneration;
	FGameplayTag AttributesSecondaryManaRegeneration;
	FGameplayTag AttributesSecondaryMaxHealth;
	FGameplayTag AttributesSecondaryMaxMana;

	// messages
	FGameplayTag MessageHealthPotion;
	FGameplayTag MessageManaPotion;
	FGameplayTag MessageHealthPotionDuration;
	FGameplayTag MessageManaPotionDuration;

	// inputs
	FGameplayTag InputTagLMB;
	FGameplayTag InputTagRMB;
	FGameplayTag InputTag1;
	FGameplayTag InputTag2;
	FGameplayTag InputTag3;
	FGameplayTag InputTag4;

private:
	static FGameplayTagSingleton Instance;
	
};
