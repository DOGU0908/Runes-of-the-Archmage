// Copyright DOGU0908


#include "AbilitySystem/CharacterAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "GameplayTagSingleton.h"
#include "AbilitySystem/AbilitySystemLibrary.h"
#include "Character/PlayerCharacterController.h"
#include "Combat/CombatInterface.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UCharacterAttributeSet::UCharacterAttributeSet()
{
	const FGameplayTagSingleton& GameplayTagSingleton = FGameplayTagSingleton::Get();

	// base attributes
	TagsToAttributes.Add(GameplayTagSingleton.AttributesBaseStrength, GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTagSingleton.AttributesBaseMagic, GetMagicAttribute);
	TagsToAttributes.Add(GameplayTagSingleton.AttributesBaseResilience, GetResilienceAttribute);
	TagsToAttributes.Add(GameplayTagSingleton.AttributesBaseVigor, GetVigorAttribute);

	// secondary attributes
	TagsToAttributes.Add(GameplayTagSingleton.AttributesSecondaryArmor, GetArmorAttribute);
	TagsToAttributes.Add(GameplayTagSingleton.AttributesSecondaryCriticalChance, GetCriticalChanceAttribute);
	TagsToAttributes.Add(GameplayTagSingleton.AttributesSecondaryHealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributes.Add(GameplayTagSingleton.AttributesSecondaryManaRegeneration, GetManaRegenerationAttribute);
}

void UCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// base attributes
	
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Mana, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Magic, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	// secondary attributes
	
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, CriticalChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
}

void UCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// PreAttributeChange should be mainly used for clamping
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
	// PreAttributeChange applies change of the NewValue to the current value, not the base value
	// i.e the base value is not being clamped
	// this results in a behavior that the health is still max value even stepping on fire area for a few seconds
	// -> the health current value was equal to max health but base value was over max health
}

void UCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties EffectProperties;
	SetEffectProperties(Data, EffectProperties);

	// can use ability system component, actor, character controller ... to implement post gameplay effect application logic

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// set the attribute base value to clamp between the range
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);

		if (LocalIncomingDamage > 0.f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			if (NewHealth > 0.f)
			{
				// activate ability if the target has the specified tag
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FGameplayTagSingleton::Get().EffectsHitReact);
				EffectProperties.TargetAbilitySystemComponent->TryActivateAbilitiesByTag(TagContainer);
			}
			else
			{
				if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(EffectProperties.TargetAvatarActor))
				{
					CombatInterface->Die();
				}
			}

			ShowFloatingText(EffectProperties, LocalIncomingDamage, UAbilitySystemLibrary::IsCriticalHit(EffectProperties.EffectContextHandle));
		}
	}
}

/*
 * Adding attributes Boilerplate
 * 1. FGameplayAttributeData member variable
 * 2. Generate the replication notify (OnRep) function with GAMEPLAYATTRIBUTE_REPNOTIFY macro
 * 3. Set the ReplicatedUsing property
 * 4. Add the new attribute variable to the replicated props by DOREPLIFETIME_CONDITION_NOTIFY macro in the virtual void GetLifetimeReplicatedProps function
 * 5. Define the following macro
 * #define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
 * 6. Create attribute accessors with the ATTRIBUTE_ACCESSORS macro
 */

void UCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Health, OldHealth);
}

void UCharacterAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Mana, OldMana);
}

void UCharacterAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Strength, OldStrength);
}

void UCharacterAttributeSet::OnRep_Magic(const FGameplayAttributeData& OldMagic) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Magic, OldMagic);
}

void UCharacterAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Resilience, OldResilience);
}

void UCharacterAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Vigor, OldVigor);
}

void UCharacterAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, MaxHealth, OldMaxHealth);
}

void UCharacterAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, MaxMana, OldMaxMana);
}

void UCharacterAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Armor, OldArmor);
}

void UCharacterAttributeSet::OnRep_CriticalChance(const FGameplayAttributeData& OldCriticalChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, CriticalChance, OldCriticalChance);
}

void UCharacterAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UCharacterAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UCharacterAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& EffectProperties)
{
	// should not use check macro, as being null is a legitimate outcome
	// for example, the actor may not have an ability system component but can trigger an effect to the character
	
	EffectProperties.EffectContextHandle = Data.EffectSpec.GetContext();

	EffectProperties.SourceAbilitySystemComponent = EffectProperties.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(EffectProperties.SourceAbilitySystemComponent) && EffectProperties.SourceAbilitySystemComponent->AbilityActorInfo.IsValid() && EffectProperties.SourceAbilitySystemComponent->AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectProperties.SourceAvatarActor = EffectProperties.SourceAbilitySystemComponent->AbilityActorInfo->AvatarActor.Get();

		EffectProperties.SourceController = EffectProperties.SourceAbilitySystemComponent->AbilityActorInfo->PlayerController.Get();

		if (!EffectProperties.SourceController && EffectProperties.SourceAvatarActor)
		{
			if (const APawn* Pawn = Cast<APawn>(EffectProperties.SourceAvatarActor))
			{
				EffectProperties.SourceController = Pawn->GetController();
			}
		}
		
		if (EffectProperties.SourceController)
		{
			EffectProperties.SourceCharacter = Cast<ACharacter>(EffectProperties.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectProperties.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();

		EffectProperties.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();

		EffectProperties.TargetCharacter = Cast<ACharacter>(EffectProperties.TargetAvatarActor);

		EffectProperties.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(EffectProperties.TargetAvatarActor);
	}
}

void UCharacterAttributeSet::ShowFloatingText(const FEffectProperties& EffectProperties, const float Damage, const bool bIsCriticalHit) const
{
	if (EffectProperties.SourceCharacter != EffectProperties.TargetCharacter)
	{
		if (APlayerCharacterController* PlayerCharacterController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(EffectProperties.SourceCharacter, 0)))
		{
			PlayerCharacterController->ShowDamageNumber(Damage, EffectProperties.TargetCharacter, bIsCriticalHit);
		}
	}
}
