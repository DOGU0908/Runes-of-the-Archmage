// Copyright DOGU0908


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/CharacterAbilitySystemComponent.h"
#include "AbilitySystem/CharacterAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UCharacterAttributeSet* CharacterAttributeSet = CastChecked<UCharacterAttributeSet>(AttributeSet);

	// initial value broadcast to ui widgets
	OnHealthChanged.Broadcast(CharacterAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(CharacterAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(CharacterAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(CharacterAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UCharacterAttributeSet* CharacterAttributeSet = CastChecked<UCharacterAttributeSet>(AttributeSet);

	// add binding to attribute change runtime
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CharacterAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CharacterAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CharacterAttributeSet->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CharacterAttributeSet->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);

	Cast<UCharacterAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& GameplayTagContainer)
		{
			for (const FGameplayTag& Tag: GameplayTagContainer)
			{
				if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Message"))))
				{
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		}
	);
}
