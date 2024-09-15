// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class UGameplayAbility;
class UGameplayEffect;

UENUM(BlueprintType)
enum class ECharacterClass: uint8
{
	Melee,
	Wizard
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Attributes")
	TSubclassOf<UGameplayEffect> BaseAttributes;

	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
};

/**
 * 
 */
UCLASS()
class RUNESOFTHEARCHMAGE_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Class Defaults")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInfo;
	
	UPROPERTY(EditDefaultsOnly, Category="Attributes")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category="Attributes")
	TSubclassOf<UGameplayEffect> BaseVitalAttributes;

	FCharacterClassDefaultInfo GetClassDefaultInfo(const ECharacterClass CharacterClass);

	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;
	
};
