// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AbilitySystemLibrary.generated.h"

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
	
};
