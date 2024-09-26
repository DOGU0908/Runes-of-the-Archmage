// Copyright DOGU0908


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "GameplayTagSingleton.h"
#include "AbilitySystem/CharacterAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Character/PlayerCharacterState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	if (UCharacterAbilitySystemComponent* CharacterAbilitySystemComponent = Cast<UCharacterAbilitySystemComponent>(AbilitySystemComponent))
	{
		BroadcastAbilityInfo(CharacterAbilitySystemComponent);
	}

	const APlayerCharacterState* PlayerCharacterState = CastChecked<APlayerCharacterState>(PlayerState);

	OnSpellPointsChanged.Broadcast(PlayerCharacterState->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	if (UCharacterAbilitySystemComponent* CharacterAbilitySystemComponent = Cast<UCharacterAbilitySystemComponent>(AbilitySystemComponent))
	{
		CharacterAbilitySystemComponent->OnAbilityStatusChanged.AddLambda(
			[this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
			{
				if (SelectedAbility.AbilityTag.MatchesTagExact(AbilityTag))
				{
					BroadcastSpellButtonClickedDelegate(CurrentSpellPoints, AbilityTag, StatusTag);
				}
				
				if (AbilityInfo)
				{
					FAbilityInfoData Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
					Info.StatusTag = StatusTag;
					OnAbilityInfoChanged.Broadcast(Info);
				}
			}
		);

		CharacterAbilitySystemComponent->OnAbilityEquipped.AddUObject(this, &USpellMenuWidgetController::OnAbilityEquipped);
	}

	APlayerCharacterState* PlayerCharacterState = CastChecked<APlayerCharacterState>(PlayerState);

	PlayerCharacterState->OnSpellPointsChangeDelegate.AddLambda(
		[this](int32 SpellPoints)
		{
			OnSpellPointsChanged.Broadcast(SpellPoints);
			CurrentSpellPoints = SpellPoints;
			BroadcastSpellButtonClickedDelegate(CurrentSpellPoints, SelectedAbility.AbilityTag, SelectedAbility.StatusTag);
		}
	);
}

void USpellMenuWidgetController::SpellButtonClicked(const FGameplayTag& AbilityTag)
{
	bWaitingForEquipSelection = false;
	
	const APlayerCharacterState* PlayerCharacterState = CastChecked<APlayerCharacterState>(PlayerState);
	const int32 SpellPoints = PlayerCharacterState->GetSpellPoints();
	const FGameplayTagSingleton GameplayTags = FGameplayTagSingleton::Get();
	
	if (UCharacterAbilitySystemComponent* CharacterAbilitySystemComponent = Cast<UCharacterAbilitySystemComponent>(AbilitySystemComponent))
	{
		FGameplayTag AbilityStatus;
		
		const bool bTagValid = AbilityTag.IsValid();
		const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
		const FGameplayAbilitySpec* AbilitySpec = CharacterAbilitySystemComponent->GetSpecByAbilityTag(AbilityTag);
		const bool bSpecValid = AbilitySpec != nullptr;

		if (!bTagValid || bTagNone || !bSpecValid)
		{
			AbilityStatus = GameplayTags.Abilities_Status_Locked;
		}
		else
		{
			AbilityStatus = CharacterAbilitySystemComponent->GetStatusFromSpec(*AbilitySpec);
		}

		BroadcastSpellButtonClickedDelegate(SpellPoints, AbilityTag, AbilityStatus);
	}
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if (UCharacterAbilitySystemComponent* CharacterAbilitySystemComponent = Cast<UCharacterAbilitySystemComponent>(AbilitySystemComponent))
	{
		CharacterAbilitySystemComponent->ServerSpendSpellPoint(SelectedAbility.AbilityTag);
	}
}

void USpellMenuWidgetController::AbilityButtonUnSelect()
{
	bWaitingForEquipSelection = false;
	
	const FGameplayTagSingleton GameplayTags = FGameplayTagSingleton::Get();
	SelectedAbility.AbilityTag = GameplayTags.Abilities_None;
	SelectedAbility.StatusTag = GameplayTags.Abilities_Status_Locked;
	OnSpellButtonClickedDelegate.Broadcast(false, false, FString(), 0);
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	WaitForEquipDelegate.Broadcast(AbilityInfo->FindAbilityInfoForTag(SelectedAbility.AbilityTag).AbilityTypeTag);
	bWaitingForEquipSelection = true;

	if (UCharacterAbilitySystemComponent* CharacterAbilitySystemComponent = Cast<UCharacterAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (FGameplayAbilitySpec* Spec = CharacterAbilitySystemComponent->GetSpecByAbilityTag(SelectedAbility.AbilityTag))
		{
			if (CharacterAbilitySystemComponent->GetStatusFromSpec(*Spec) == FGameplayTagSingleton::Get().Abilities_Status_Equipped)
			{
				SelectedInputTag = CharacterAbilitySystemComponent->GetInputTagFromSpec(*Spec);
			}
		}
	}
}

void USpellMenuWidgetController::InputSpellButtonClicked(const FGameplayTag& InputTag, const FGameplayTag& AbilityTypeTag)
{
	if (!bWaitingForEquipSelection)
	{
		return;
	}
	if (!AbilityInfo->FindAbilityInfoForTag(SelectedAbility.AbilityTag).AbilityTypeTag.MatchesTagExact(AbilityTypeTag))
	{
		return;
	}

	if (UCharacterAbilitySystemComponent* CharacterAbilitySystemComponent = Cast<UCharacterAbilitySystemComponent>(AbilitySystemComponent))
	{
		CharacterAbilitySystemComponent->ServerEquipAbility(SelectedAbility.AbilityTag, InputTag);
	}
}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag,
	const FGameplayTag& InputTag, const FGameplayTag& PrevInputTag)
{
	bWaitingForEquipSelection = false;

	const FGameplayTagSingleton& GameplayTags = FGameplayTagSingleton::Get();

	// broadcast an empty ability info data to erase previous input slot
	FAbilityInfoData LastInputSlotInfo;
	LastInputSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastInputSlotInfo.InputTag = PrevInputTag;
	LastInputSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	OnAbilityInfoChanged.Broadcast(LastInputSlotInfo);

	// broadcast new ability info data to assign input slot
	FAbilityInfoData Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = StatusTag;
	Info.InputTag = InputTag;
	OnAbilityInfoChanged.Broadcast(Info);

	OnSpellReassignedDelegate.Broadcast(AbilityTag);
	AbilityButtonUnSelect();
}

void USpellMenuWidgetController::BroadcastSpellButtonClickedDelegate(const int32 SpellPoints, const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
{
	const FGameplayTagSingleton GameplayTags = FGameplayTagSingleton::Get();
	SelectedAbility.AbilityTag = AbilityTag;
	SelectedAbility.StatusTag = StatusTag;

	FString Name;
	int32 Level;
	if (UCharacterAbilitySystemComponent* CharacterAbilitySystemComponent = Cast<UCharacterAbilitySystemComponent>(AbilitySystemComponent))
	{
		CharacterAbilitySystemComponent->GetInfoByAbilityTag(AbilityTag, Name, Level);
	}

	if (StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Equipped) || StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		OnSpellButtonClickedDelegate.Broadcast(SpellPoints > 0, true, Name, Level);
	}
	else if (StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		OnSpellButtonClickedDelegate.Broadcast(SpellPoints > 0, false, Name, Level);
	}
	else if (StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Locked))
	{
		OnSpellButtonClickedDelegate.Broadcast(false, false, Name, Level);
	}
}
