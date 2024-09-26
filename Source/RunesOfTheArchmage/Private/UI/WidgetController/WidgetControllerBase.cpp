// Copyright DOGU0908


#include "UI/WidgetController/WidgetControllerBase.h"

#include "AbilitySystem/CharacterAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"

void UWidgetControllerBase::SetWidgetControllerParams(const FWidgetControllerParams& WidgetControllerParams)
{
	PlayerController = WidgetControllerParams.PlayerController;
	PlayerState = WidgetControllerParams.PlayerState;
	AbilitySystemComponent = WidgetControllerParams.AbilitySystemComponent;
	AttributeSet = WidgetControllerParams.AttributeSet;
}

void UWidgetControllerBase::BroadcastInitialValues()
{
}

void UWidgetControllerBase::BindCallbacksToDependencies()
{
}

void UWidgetControllerBase::BroadcastAbilityInfo(UCharacterAbilitySystemComponent* CharacterAbilitySystemComponent) const
{
	if (!CharacterAbilitySystemComponent->bGivenStartupAbilities)
	{
		return;
	}

	FForEachAbility AbilityDelegate;
	AbilityDelegate.BindLambda(
		[this, CharacterAbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)
		{
			FAbilityInfoData AbilityInfoData = AbilityInfo->FindAbilityInfoForTag(CharacterAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
			// check the ability input tag
			AbilityInfoData.InputTag = CharacterAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
			AbilityInfoData.StatusTag = CharacterAbilitySystemComponent->GetStatusFromSpec(AbilitySpec);
			OnAbilityInfoChanged.Broadcast(AbilityInfoData);
		}
	);
	CharacterAbilitySystemComponent->ForEachAbility(AbilityDelegate);
}
