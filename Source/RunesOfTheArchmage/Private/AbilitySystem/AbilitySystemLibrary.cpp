// Copyright DOGU0908


#include "AbilitySystem/AbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilityTypes.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/PlayerCharacterState.h"
#include "Combat/CombatInterface.h"
#include "Engine/OverlapResult.h"
#include "Game/RunesOfTheArchmageGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/HUDBase.h"
#include "UI/WidgetController/WidgetControllerBase.h"

bool UAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldObject,
	FWidgetControllerParams& OutWidgetControllerParams, AHUDBase*& OutHUD)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldObject, 0))
	{
		OutHUD = Cast<AHUDBase>(PlayerController->GetHUD());
		if (OutHUD)
		{
			const APlayerCharacterState* PlayerCharacterState = PlayerController->GetPlayerState<APlayerCharacterState>();
			
			OutWidgetControllerParams.AttributeSet = PlayerCharacterState->GetAttributeSet();
			OutWidgetControllerParams.AbilitySystemComponent = PlayerCharacterState->GetAbilitySystemComponent();
			OutWidgetControllerParams.PlayerState = PlayerController->GetPlayerState<APlayerCharacterState>();
			OutWidgetControllerParams.PlayerController = PlayerController;
			
			return true;
		}
	}

	return false;
}

UOverlayWidgetController* UAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldObject)
{
	FWidgetControllerParams WidgetControllerParams;
	AHUDBase* HUD = nullptr;
	if (MakeWidgetControllerParams(WorldObject, WidgetControllerParams, HUD))
	{
		return HUD->GetOverlayWidgetController(WidgetControllerParams);
	}

	return nullptr;
}

UAttributeMenuWidgetController* UAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldObject)
{
	FWidgetControllerParams WidgetControllerParams;
	AHUDBase* HUD = nullptr;
	if (MakeWidgetControllerParams(WorldObject, WidgetControllerParams, HUD))
	{
		return HUD->GetAttributeMenuWidgetController(WidgetControllerParams);
	}

	return nullptr;
}

USpellMenuWidgetController* UAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldObject)
{
	FWidgetControllerParams WidgetControllerParams;
	AHUDBase* HUD = nullptr;
	if (MakeWidgetControllerParams(WorldObject, WidgetControllerParams, HUD))
	{
		return HUD->GetSpellMenuWidgetController(WidgetControllerParams);
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
	UAbilitySystemComponent* AbilitySystemComponent, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldObject);
	if (!CharacterClassInfo)
	{
		return;
	}
	
	for (auto CommonAbility: CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(CommonAbility, 1);
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(AbilitySystemComponent->GetAvatarActor()))
	{
		for (auto StartupAbility: CharacterClassInfo->GetClassDefaultInfo(CharacterClass).StartupAbilities)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(StartupAbility, CombatInterface->GetCharacterLevel());
			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
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

void UAbilitySystemLibrary::GetLiveCharactersWithinRadius(const UObject* WorldObject,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, const float Radius,
	const FVector& CenterLocation)
{
	// from UGameplayStatics::ApplyRadialDamageWithFalloff
	FCollisionQueryParams SphereParams;

	SphereParams.AddIgnoredActors(ActorsToIgnore);

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> OverlapResults;
		World->OverlapMultiByObjectType(OverlapResults, CenterLocation, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);

		for (auto& OverlapResult: OverlapResults)
		{
			if (OverlapResult.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(OverlapResult.GetActor()))
			{
				OutOverlappingActors.AddUnique(OverlapResult.GetActor());
			}
		}
	}
}

bool UAbilitySystemLibrary::IsNotFriendlyUnit(const AActor* FirstActor, const AActor* SecondActor)
{
	return FirstActor->ActorHasTag(FName("Player")) != SecondActor->ActorHasTag(FName("Player"));
}

UCharacterClassInfo* UAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldObject)
{
	const ARunesOfTheArchmageGameModeBase* RunesOfTheArchmageGameMode = Cast<ARunesOfTheArchmageGameModeBase>(UGameplayStatics::GetGameMode(WorldObject));

	if (!RunesOfTheArchmageGameMode)
	{
		return nullptr;
	}
	
	return RunesOfTheArchmageGameMode->CharacterClassInfo;
}

UAbilityInfo* UAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldObject)
{
	const ARunesOfTheArchmageGameModeBase* RunesOfTheArchmageGameMode = Cast<ARunesOfTheArchmageGameModeBase>(UGameplayStatics::GetGameMode(WorldObject));

	if (!RunesOfTheArchmageGameMode)
	{
		return nullptr;
	}
	
	return RunesOfTheArchmageGameMode->AbilityInfo;
}

int32 UAbilitySystemLibrary::GetExpRewardByClassAndLevel(const UObject* WorldObject, ECharacterClass CharacterClass,
                                                         int32 Level)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldObject);
	if (!CharacterClassInfo)
	{
		return 0;
	}

	const FCharacterClassDefaultInfo& ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const float ExpReward = ClassDefaultInfo.ExpReward.GetValueAtLevel(Level);

	return static_cast<int32>(ExpReward);
}
