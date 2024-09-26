// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AbilitySystemLibrary.generated.h"

class UAbilityInfo;
class AHUDBase;
struct FWidgetControllerParams;
class USpellMenuWidgetController;
class UCharacterClassInfo;
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
	static bool MakeWidgetControllerParams(const UObject* WorldObject, FWidgetControllerParams& OutWidgetControllerParams, AHUDBase*& OutHUD);
	
	UFUNCTION(BlueprintPure, Category="AbilitySystemLibrary")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldObject);

	UFUNCTION(BlueprintPure, Category="AbilitySystemLibrary")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldObject);

	UFUNCTION(BlueprintPure, Category="AbilitySystemLibrary")
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldObject);

	UFUNCTION(BlueprintCallable, Category="AbilitySystemLibrary")
	static void InitializeDefaultAttributes(const UObject* WorldObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* AbilitySystemComponent);

	UFUNCTION(BlueprintCallable, Category="AbilitySystemLibrary")
	static void GiveStartupAbilities(const UObject* WorldObject, UAbilitySystemComponent* AbilitySystemComponent, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintPure, Category="AbilitySystemLibrary")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category="AbilitySystemLibrary")
	static void GetLiveCharactersWithinRadius(const UObject* WorldObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, const float Radius, const FVector& CenterLocation);

	UFUNCTION(BlueprintPure, Category="AbilitySystemLibrary")
	static bool IsNotFriendlyUnit(const AActor* FirstActor, const AActor* SecondActor);

	UFUNCTION(BlueprintCallable, Category="AbilitySystemLibrary")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldObject);

	UFUNCTION(BlueprintCallable, Category="AbilitySystemLibrary")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldObject);

	static int32 GetExpRewardByClassAndLevel(const UObject* WorldObject, ECharacterClass CharacterClass, int32 Level);
	
};
