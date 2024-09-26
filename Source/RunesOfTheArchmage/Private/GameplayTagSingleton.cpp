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

	// temporary attributes
	Instance.AttributesMetaIncomingExp = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Meta.IncomingExp"));
	
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
	Instance.InputTagPassive = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Passive"));

	// event montages
	Instance.EventMontageAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Montage.Attack"));
	Instance.EventMontageAttackMelee = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Montage.Attack.Melee"));
	Instance.EventMontageAttackWizard = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Montage.Attack.Wizard"));
	
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

	// abilities
	Instance.Abilities_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.None")); // a null ability tag
	
	Instance.Abilities_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Fire"));
	Instance.Abilities_FireStorm = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.FireStorm"));
	Instance.Abilities_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Ice"));
	Instance.Abilities_IceStorm = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.IceStorm"));
	Instance.Abilities_Thunder = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Thunder"));
	Instance.Abilities_ThunderStorm = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.ThunderStorm"));

	Instance.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Locked"));
	Instance.Abilities_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Eligible"));
	Instance.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Unlocked"));
	Instance.Abilities_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Equipped"));

	Instance.Abilities_Type_Offensive = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.Offensive"));
	Instance.Abilities_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.Passive"));
	Instance.Abilities_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.None"));

	// cooldowns
	Instance.Cooldown_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Fire"));
	Instance.Cooldown_FireStorm = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.FireStorm"));
	Instance.Cooldown_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Ice"));
	Instance.Cooldown_IceStorm = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.IceStorm"));
	Instance.Cooldown_Thunder = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Thunder"));
	Instance.Cooldown_ThunderStorm = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.ThunderStorm"));
}
