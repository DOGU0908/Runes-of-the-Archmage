// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "InputDataAsset.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FInputActionData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
	
};

/**
 * 
 */
UCLASS()
class RUNESOFTHEARCHMAGE_API UInputDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FInputActionData> AbilityInputActionData;

	const UInputAction* FindAbilityInputActionByTag(const FGameplayTag& InputTag, const bool bLogNotFound = false) const;
	
};
