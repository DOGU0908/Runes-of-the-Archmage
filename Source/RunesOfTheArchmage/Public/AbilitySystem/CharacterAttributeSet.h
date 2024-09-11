// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() {}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceAbilitySystemComponent = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetAbilitySystemComponent = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};

// generic static function pointer
template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

/**
 * 
 */
UCLASS()
class RUNESOFTHEARCHMAGE_API UCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UCharacterAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;
	
	/*
	 * Base Attributes
	 */
	
	// health
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Health, Category="Base Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Health);
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	// mana
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Mana, Category="Base Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Mana);

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	// strength
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Strength, Category="Base Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Strength);

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	// magic
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Magic, Category="Base Attributes")
	FGameplayAttributeData Magic;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Magic);

	UFUNCTION()
	void OnRep_Magic(const FGameplayAttributeData& OldMagic) const;

	// resilience
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Resilience, Category="Base Attributes")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Resilience);

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;

	// vigor
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Vigor, Category="Base Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Vigor);

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

	/*
	 * Secondary Attributes
	 */

	// max health
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth, Category="Base Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxHealth);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	// max mana
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxMana, Category="Base Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxMana);

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	// armor
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Armor, Category="Base Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Armor);

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;

	// critical chance
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CriticalChance, Category="Base Attributes")
	FGameplayAttributeData CriticalChance;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, CriticalChance);

	UFUNCTION()
	void OnRep_CriticalChance(const FGameplayAttributeData& OldCriticalChance) const;

	// health regeneration
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_HealthRegeneration, Category="Base Attributes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, HealthRegeneration);

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;

	// mana regeneration
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ManaRegeneration, Category="Base Attributes")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, ManaRegeneration);

	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;

	/*
	 * resistances
	 */

	// fire resistance
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_FireResistance, Category="Resistance Attributes")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, FireResistance);

	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const;

	// ice resistance
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_IceResistance, Category="Resistance Attributes")
	FGameplayAttributeData IceResistance;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, IceResistance);

	UFUNCTION()
	void OnRep_IceResistance(const FGameplayAttributeData& OldIceResistance) const;

	// thunder resistance
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ThunderResistance, Category="Resistance Attributes")
	FGameplayAttributeData ThunderResistance;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, ThunderResistance);

	UFUNCTION()
	void OnRep_ThunderResistance(const FGameplayAttributeData& OldThunderResistance) const;

	// physical resistance
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_PhysicalResistance, Category="Resistance Attributes")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, PhysicalResistance);

	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const;
	
	/*
	 * Temp Attributes - not replicated
	 */

	UPROPERTY(BlueprintReadOnly, Category="Temp Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, IncomingDamage);

private:
	static void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& EffectProperties);

	void ShowFloatingText(const FEffectProperties& EffectProperties, const float Damage, const bool bIsCriticalHit) const;
};
