// Copyright DOGU0908


#include "Character/CharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/CharacterAbilitySystemComponent.h"

ACharacterBase::ACharacterBase()
{
 	PrimaryActorTick.bCanEverTick = false;
	
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* ACharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacterBase::InitAbilityActorInfo()
{
}

void ACharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	
	FGameplayEffectContextHandle GameplayEffectContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	GameplayEffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle GameplayEffectSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, GameplayEffectContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());
}

void ACharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultBaseAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	// set health and mana after setting max health and max mana based on base attributes
	ApplyEffectToSelf(DefaultBaseVitalAttributes, 1.f);
}

void ACharacterBase::AddCharacterAbilities() const
{
	// only add in server
	if (!HasAuthority())
	{
		return;
	}

	UCharacterAbilitySystemComponent* CharacterAbilitySystemComponent = CastChecked<UCharacterAbilitySystemComponent>(AbilitySystemComponent);
	CharacterAbilitySystemComponent->AddCharacterAbilities(StartAbilities);
}

FVector ACharacterBase::GetCombatSocketLocation()
{
	check(GetMesh());
	return GetMesh()->GetSocketLocation(CombatSocketName);
}
