// Copyright DOGU0908


#include "Character/CharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/CharacterAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"

ACharacterBase::ACharacterBase()
{
 	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetGenerateOverlapEvents(true);
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

void ACharacterBase::InitializeDefaultAttributes() const
{
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
