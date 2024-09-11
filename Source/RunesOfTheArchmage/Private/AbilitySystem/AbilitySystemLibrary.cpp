// Copyright DOGU0908


#include "AbilitySystem/AbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilityTypes.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/PlayerCharacterState.h"
#include "Game/RunesOfTheArchmageGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/HUDBase.h"
#include "UI/WidgetController/WidgetControllerBase.h"

UOverlayWidgetController* UAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldObject, 0))
	{
		if (AHUDBase* HUD = Cast<AHUDBase>(PlayerController->GetHUD()))
		{
			APlayerCharacterState* PlayerCharacterState = PlayerController->GetPlayerState<APlayerCharacterState>();
			UAbilitySystemComponent* AbilitySystemComponent = PlayerCharacterState->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = PlayerCharacterState->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerCharacterState, AbilitySystemComponent, AttributeSet);
			return HUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}

UAttributeMenuWidgetController* UAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldObject, 0))
	{
		if (AHUDBase* HUD = Cast<AHUDBase>(PlayerController->GetHUD()))
		{
			APlayerCharacterState* PlayerCharacterState = PlayerController->GetPlayerState<APlayerCharacterState>();
			UAbilitySystemComponent* AbilitySystemComponent = PlayerCharacterState->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = PlayerCharacterState->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerCharacterState, AbilitySystemComponent, AttributeSet);
			return HUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}

void UAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* AbilitySystemComponent)
{
	const ARunesOfTheArchmageGameModeBase* RunesOfTheArchmageGameMode = Cast<ARunesOfTheArchmageGameModeBase>(UGameplayStatics::GetGameMode(WorldObject));

	if (!RunesOfTheArchmageGameMode)
	{
		return;
	}

	AActor* AvatarActor = AbilitySystemComponent->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = RunesOfTheArchmageGameMode->CharacterClassInfo;
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle BaseAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();
	BaseAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle BaseAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(ClassDefaultInfo.BaseAttributes, Level, BaseAttributesContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*BaseAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle BaseAttributesVitalContextHandle = AbilitySystemComponent->MakeEffectContext();
	BaseAttributesVitalContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle BaseAttributesVitalSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(CharacterClassInfo->BaseVitalAttributes, Level, BaseAttributesVitalContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*BaseAttributesVitalSpecHandle.Data.Get());
}

void UAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldObject,
	UAbilitySystemComponent* AbilitySystemComponent)
{
	const ARunesOfTheArchmageGameModeBase* RunesOfTheArchmageGameMode = Cast<ARunesOfTheArchmageGameModeBase>(UGameplayStatics::GetGameMode(WorldObject));

	if (!RunesOfTheArchmageGameMode)
	{
		return;
	}
	
	UCharacterClassInfo* CharacterClassInfo = RunesOfTheArchmageGameMode->CharacterClassInfo;
	for (auto CommonAbility: CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(CommonAbility, 1);
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}

bool UAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FGameplayEffectContextBase* EffectContext = static_cast<const FGameplayEffectContextBase*>(EffectContextHandle.Get()))
	{
		return EffectContext->IsCriticalHit();
	}

	return false;
}
