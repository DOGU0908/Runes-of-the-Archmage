// Copyright DOGU0908


#include "Character/PlayerCharacterState.h"

#include "AbilitySystem/CharacterAbilitySystemComponent.h"
#include "AbilitySystem/CharacterAttributeSet.h"
#include "Net/UnrealNetwork.h"

APlayerCharacterState::APlayerCharacterState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCharacterAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	/*
	 * Replication Mode
	 * Full - Single Player
	 * Mixed - Multi Player, Player Controlled
	 * Minimal - Multi Player, AI Controlled
	 */
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>("AttributeSet");
	
	NetUpdateFrequency = 50.f;
}

UAbilitySystemComponent* APlayerCharacterState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* APlayerCharacterState::GetAttributeSet() const
{
	return AttributeSet;
}

void APlayerCharacterState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacterState, Level);
}

void APlayerCharacterState::OnRep_Level(int32 OldLevel) const
{
}
