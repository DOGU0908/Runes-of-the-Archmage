// Copyright DOGU0908


#include "GameplayTagSingleton.h"

#include "GameplayTagsManager.h"

FGameplayTagSingleton FGameplayTagSingleton::Instance;

void FGameplayTagSingleton::InitializeNativeGameplayTags()
{
	// base attributes
	Instance.AttributesBaseHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Base.Health"));
	Instance.AttributesBaseMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Base.Mana"));
	Instance.AttributesBaseStrength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Base.Strength"));
	Instance.AttributesBaseMagic = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Base.Magic"));
	Instance.AttributesBaseResilience = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Base.Resilience"));
	Instance.AttributesBaseVigor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Base.Vigor"));

	// secondary attributes
	Instance.AttributesSecondaryArmor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"));
	Instance.AttributesSecondaryCriticalChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalChance"));
	Instance.AttributesSecondaryHealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.HealthRegeneration"));
	Instance.AttributesSecondaryManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ManaRegeneration"));
	Instance.AttributesSecondaryMaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"));
	Instance.AttributesSecondaryMaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"));

	// messages
	Instance.MessageHealthPotion = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message.HealthPotion"));
	Instance.MessageManaPotion = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message.ManaPotion"));
	Instance.MessageHealthPotionDuration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message.HealthPotionDuration"));
	Instance.MessageManaPotionDuration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message.ManaPotionDuration"));
	
	// inputs
	Instance.InputTagLMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"));
	Instance.InputTagRMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"));
	Instance.InputTag1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.1"));
	Instance.InputTag2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.2"));
	Instance.InputTag3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.3"));
	Instance.InputTag4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.4"));

	// event montages
	Instance.EventMontageAttack01 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Montage.Attack01"));

	// combats
	Instance.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"));
	Instance.EffectsHitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"));

	// damage types
	Instance.DamageTypeFire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Type.Fire"));
	Instance.DamageTypeIce = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Type.Ice"));
	Instance.DamageTypeThunder = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Type.Thunder"));
	Instance.DamageTypePhysical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Type.Physical"));

	// resistances
	Instance.AttributesResistancesFire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Fire"));
	Instance.AttributesResistancesIce = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Ice"));
	Instance.AttributesResistancesThunder = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Thunder"));
	Instance.AttributesResistancesPhysical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Physical"));

	// map of damage type to resistances
	Instance.DamageTypesToResistances.Add(Instance.DamageTypeFire, Instance.AttributesResistancesFire);
	Instance.DamageTypesToResistances.Add(Instance.DamageTypeIce, Instance.AttributesResistancesIce);
	Instance.DamageTypesToResistances.Add(Instance.DamageTypeThunder, Instance.AttributesResistancesThunder);
	Instance.DamageTypesToResistances.Add(Instance.DamageTypePhysical, Instance.AttributesResistancesPhysical);
}
