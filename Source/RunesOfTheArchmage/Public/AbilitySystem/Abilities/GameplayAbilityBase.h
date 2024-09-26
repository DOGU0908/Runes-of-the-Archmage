// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class RUNESOFTHEARCHMAGE_API UGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartupInputTag;

	FString GetName() const;

private:
	UPROPERTY(EditDefaultsOnly)
	FString Name;
};
