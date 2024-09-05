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
}
