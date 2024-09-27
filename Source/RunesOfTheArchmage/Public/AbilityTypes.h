// Copyright DOGU0908

#pragma once

#include "GameplayEffectTypes.h"
#include "AbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams() {}

	UPROPERTY()
	TObjectPtr<UObject> WorldObject = nullptr;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	UPROPERTY()
	float BaseDamage = 0.f;

	UPROPERTY()
	float AbilityLevel = 1.f;

	UPROPERTY()
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY()
	float DebuffChance = 0.f;

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;

	UPROPERTY()
	float DebuffFrequency = 0.f;
};

USTRUCT(BlueprintType)
struct FGameplayEffectContextBase : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	virtual UScriptStruct* GetScriptStruct() const override;

	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

	bool IsCriticalHit() const;
	void SetIsCriticalHit(const bool bInIsCriticalHit);
	bool IsSuccessfulDebuff() const {return bIsSuccessfulDebuff;}
	void SetIsSuccessfulDebuff(const bool bInIsSuccessfulDebuff);
	float GetDebuffDamage() const {return DebuffDamage;}
	void SetDebuffDamage(const float InDebuffDamage);
	float GetDebuffDuration() const {return DebuffDuration;}
	void SetDebuffDuration(const float InDebuffDuration);
	float GetDebuffFrequency() const {return DebuffFrequency;}
	void SetDebuffFrequency(const float InDebuffFrequency);
	TSharedPtr<FGameplayTag> GetDamageType() const {return DamageType;}
	void SetDamageType(const FGameplayTag& InDamageType);

	virtual FGameplayEffectContextBase* Duplicate() const override;
	
protected:
	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	bool bIsSuccessfulDebuff = false;

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;

	UPROPERTY()
	float DebuffFrequency = 0.f;
	
	TSharedPtr<FGameplayTag> DamageType;
	
};

template<>
struct TStructOpsTypeTraits<FGameplayEffectContextBase> : public TStructOpsTypeTraitsBase2<FGameplayEffectContextBase>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};	
};
