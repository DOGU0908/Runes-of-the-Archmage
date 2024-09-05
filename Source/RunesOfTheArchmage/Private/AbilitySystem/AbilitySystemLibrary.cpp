// Copyright DOGU0908


#include "AbilitySystem/AbilitySystemLibrary.h"

#include "Character/PlayerCharacterState.h"
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
