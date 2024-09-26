// Copyright DOGU0908


#include "UI/WidgetController/OverlayWidgetController.h"

#include "GameplayTagSingleton.h"
#include "AbilitySystem/CharacterAbilitySystemComponent.h"
#include "AbilitySystem/CharacterAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Character/PlayerCharacterState.h"

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
	APlayerCharacterState* PlayerCharacterState = CastChecked<APlayerCharacterState>(PlayerState);

	PlayerCharacterState->OnExpChangeDelegate.AddUObject(this, &UOverlayWidgetController::OnExpChanged);
	PlayerCharacterState->OnLevelChangeDelegate.AddLambda(
		[this](int32 NewLevel)
		{
			OnPlayerLevelChanged.Broadcast(NewLevel);
		}
	);
	
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

	if (UCharacterAbilitySystemComponent* CharacterAbilitySystemComponent = Cast<UCharacterAbilitySystemComponent>(AbilitySystemComponent))
	{
		CharacterAbilitySystemComponent->EffectAssetTags.AddLambda(
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

		CharacterAbilitySystemComponent->OnAbilityEquipped.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);

		if (CharacterAbilitySystemComponent->bGivenStartupAbilities)
		{
			OnInitializeStartupAbilities(CharacterAbilitySystemComponent);
		}
		else
		{
			CharacterAbilitySystemComponent->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitializeStartupAbilities);
		}
	}
}

void UOverlayWidgetController::OnInitializeStartupAbilities(UCharacterAbilitySystemComponent* CharacterAbilitySystemComponent) const
{
	BroadcastAbilityInfo(CharacterAbilitySystemComponent);
}

void UOverlayWidgetController::OnExpChanged(int32 NewExp) const
{
	const APlayerCharacterState* PlayerCharacterState = CastChecked<APlayerCharacterState>(PlayerState);

	const ULevelUpInfo* LevelUpInfo = PlayerCharacterState->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Level up info is not found in player state"));

	const int32 Level = LevelUpInfo->FindLevelByExp(NewExp);
	// if not max level
	if (Level <= LevelUpInfo->LevelUpInfo.Num())
	{
		const int32 PreviousLevelUpRequirement = Level > 1 ? LevelUpInfo->LevelUpInfo[Level - 2].LevelUpRequirement : 0;
		
		const int32 ThisLevelUpRequirement = LevelUpInfo->LevelUpInfo[Level - 1].LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 ThisLevelExp = NewExp - PreviousLevelUpRequirement;

		const float ExpBarPercent = static_cast<float>(ThisLevelExp) / static_cast<float>(ThisLevelUpRequirement);
		OnExpPercentChanged.Broadcast(ExpBarPercent);
	}
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag,
	const FGameplayTag& InputTag, const FGameplayTag& PrevInputTag) const
{
	const FGameplayTagSingleton& GameplayTags = FGameplayTagSingleton::Get();

	FAbilityInfoData LastInputSlotInfo;
	LastInputSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastInputSlotInfo.InputTag = PrevInputTag;
	LastInputSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	OnAbilityInfoChanged.Broadcast(LastInputSlotInfo);

	FAbilityInfoData Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = StatusTag;
	Info.InputTag = InputTag;
	OnAbilityInfoChanged.Broadcast(Info);
}
