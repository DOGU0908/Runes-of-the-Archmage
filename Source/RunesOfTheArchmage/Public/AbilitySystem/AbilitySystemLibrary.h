// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AbilitySystemLibrary.generated.h"

struct FGameplayEffectContextHandle;
class UAbilitySystemComponent;
enum class ECharacterClass : uint8;
class UAttributeMenuWidgetController;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class RUNESOFTHEARCHMAGE_API UAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="AbilitySystemLibrary")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldObject);

	UFUNCTION(BlueprintPure, Category="AbilitySystemLibrary")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldObject);

	UFUNCTION(BlueprintCallable, Category="AbilitySystemLibrary")
	static void InitializeDefaultAttributes(const UObject* WorldObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* AbilitySystemComponent);

	UFUNCTION(BlueprintCallable, Category="AbilitySystemLibrary")
	static void GiveStartupAbilities(const UObject* WorldObject, UAbilitySystemComponent* AbilitySystemComponent);

	UFUNCTION(BlueprintPure, Category="AbilitySystemLibrary")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
};
