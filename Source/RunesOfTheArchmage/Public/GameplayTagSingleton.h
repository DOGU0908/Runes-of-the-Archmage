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

	// temporary attributes
	FGameplayTag AttributesMetaIncomingExp;

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
	FGameplayTag InputTagPassive;

	// event montages
	FGameplayTag EventMontageAttack;
	FGameplayTag EventMontageAttackMelee;
	FGameplayTag EventMontageAttackWizard;

	// combats
	FGameplayTag Damage;
	FGameplayTag EffectsHitReact;

	// damage types
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
	FGameplayTag DamageTypeFire;
	FGameplayTag DamageTypeIce;
	FGameplayTag DamageTypeThunder;
	FGameplayTag DamageTypePhysical;

	// resistances
	FGameplayTag AttributesResistancesFire;
	FGameplayTag AttributesResistancesIce;
	FGameplayTag AttributesResistancesThunder;
	FGameplayTag AttributesResistancesPhysical;

	// damage bonuses
	FGameplayTag AttributesBonusesFire;
	FGameplayTag AttributesBonusesIce;
	FGameplayTag AttributesBonusesThunder;
	FGameplayTag AttributesBonusesPhysical;
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDamageBonuses;
	
	// debuffs
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;
	FGameplayTag DebuffBurn;
	FGameplayTag DebuffFreeze;
	FGameplayTag DebuffShock;
	FGameplayTag DebuffBleeding;

	FGameplayTag DebuffPropertyChance;
	FGameplayTag DebuffPropertyDamage;
	FGameplayTag DebuffPropertyDuration;
	FGameplayTag DebuffPropertyFrequency;

	// abilities
	FGameplayTag Abilities_None;
	
	FGameplayTag Abilities_Fire;
	FGameplayTag Abilities_FireStorm;
	FGameplayTag Abilities_Ice;
	FGameplayTag Abilities_IceStorm;
	FGameplayTag Abilities_Thunder;
	FGameplayTag Abilities_ThunderStorm;

	FGameplayTag Abilities_Passive_Physical;
	FGameplayTag Abilities_Passive_Fire;
	FGameplayTag Abilities_Passive_Ice;
	FGameplayTag Abilities_Passive_Thunder;

	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Unlocked;
	FGameplayTag Abilities_Status_Equipped;

	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_None;

	// cooldowns
	FGameplayTag Cooldown_Fire;
	FGameplayTag Cooldown_FireStorm;
	FGameplayTag Cooldown_Ice;
	FGameplayTag Cooldown_IceStorm;
	FGameplayTag Cooldown_Thunder;
	FGameplayTag Cooldown_ThunderStorm;

	// player ability block tags
	FGameplayTag Player_Block_AbilityActivation;

private:
	static FGameplayTagSingleton Instance;
	
};
