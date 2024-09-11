// Copyright DOGU0908

#pragma once

#include "GameplayEffectTypes.h"
#include "AbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FGameplayEffectContextBase : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	virtual UScriptStruct* GetScriptStruct() const override;

	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

	bool IsCriticalHit() const;
	void SetIsCriticalHit(const bool bInIsCriticalHit);

	virtual FGameplayEffectContextBase* Duplicate() const override;
	
protected:
	UPROPERTY()
	bool bIsCriticalHit = false;
	
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
