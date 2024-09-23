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
	DOREPLIFETIME(APlayerCharacterState, Exp);
}

void APlayerCharacterState::AddLevel(const int32 InLevel)
{
	Level += InLevel;

	OnLevelChangeDelegate.Broadcast(Level);
}

void APlayerCharacterState::SetLevel(const int32 InLevel)
{
	Level = InLevel;

	OnLevelChangeDelegate.Broadcast(Level);
}

void APlayerCharacterState::AddToExp(const int32 InExp)
{
	Exp += InExp;

	OnExpChangeDelegate.Broadcast(Exp);
}

void APlayerCharacterState::SetExp(const int32 InExp)
{
	Exp = InExp;
	
	OnExpChangeDelegate.Broadcast(Exp);
}

void APlayerCharacterState::OnRep_Level(int32 OldLevel) const
{
	OnLevelChangeDelegate.Broadcast(Level);
}

void APlayerCharacterState::OnRep_Exp(int32 OldExp) const
{
	// exp change only occurs in server, then client is replicated and calls this function
	OnExpChangeDelegate.Broadcast(Exp);
}
