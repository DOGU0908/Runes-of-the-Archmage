// Copyright DOGU0908


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/CharacterAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UCharacterAttributeSet* CharacterAttributeSet = CastChecked<UCharacterAttributeSet>(AttributeSet);

	check(AttributeInfo);
	
	for (auto& Pair: CharacterAttributeSet->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UCharacterAttributeSet* CharacterAttributeSet = CastChecked<UCharacterAttributeSet>(AttributeSet);

	check(AttributeInfo);

	for (auto& Pair: CharacterAttributeSet->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& GameplayAttribute) const
{
	FBaseAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = GameplayAttribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
