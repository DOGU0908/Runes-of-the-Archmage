// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTagSingleton.h"
#include "UI/WidgetController/WidgetControllerBase.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnSpellButtonClicked, bool, bSpendPointButtonEnabled, bool, bEquipButtonEnabled, FString, Name, int32, Level);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquip, const FGameplayTag&, AbilityType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpellReassigned, const FGameplayTag&, AbilityTag);

struct FSelectedAbility
{
	FGameplayTag AbilityTag = FGameplayTag();
	FGameplayTag StatusTag = FGameplayTag();
};
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class RUNESOFTHEARCHMAGE_API USpellMenuWidgetController : public UWidgetControllerBase
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChanged OnSpellPointsChanged;

	UFUNCTION(BlueprintCallable)
	void SpellButtonClicked(const FGameplayTag& AbilityTag);

	UPROPERTY(BlueprintAssignable)
	FOnSpellButtonClicked OnSpellButtonClickedDelegate;

	UFUNCTION(BlueprintCallable)
	void SpendPointButtonPressed();

	UFUNCTION(BlueprintCallable)
	void AbilityButtonUnSelect();

	UPROPERTY(BlueprintAssignable)
	FWaitForEquip WaitForEquipDelegate;

	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed();

	UFUNCTION(BlueprintCallable)
	void InputSpellButtonClicked(const FGameplayTag& InputTag, const FGameplayTag& AbilityTypeTag);

	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& InputTag, const FGameplayTag& PrevInputTag);

	UPROPERTY(BlueprintAssignable)
	FOnSpellReassigned OnSpellReassignedDelegate;

private:
	FSelectedAbility SelectedAbility = {FGameplayTagSingleton::Get().Abilities_None, FGameplayTagSingleton::Get().Abilities_Status_Locked};
	int32 CurrentSpellPoints = 0;
	void BroadcastSpellButtonClickedDelegate(const int32 SpellPoints, const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag);

	bool bWaitingForEquipSelection = false;
	FGameplayTag SelectedInputTag;
};
