// Copyright DOGU0908


#include "Character/CharacterBase.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagSingleton.h"
#include "AbilitySystem/CharacterAbilitySystemComponent.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "RunesOfTheArchmage/RunesOfTheArchmage.h"

ACharacterBase::ACharacterBase()
{
 	PrimaryActorTick.bCanEverTick = false;

	BurnDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("BurnDebuffComponent");
	BurnDebuffComponent->SetupAttachment(GetRootComponent());
	BurnDebuffComponent->DebuffTag = FGameplayTagSingleton::Get().DebuffBurn;
	
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
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

UAnimMontage* ACharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void ACharacterBase::Die()
{
	MulticastHandleDeath();
}

bool ACharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* ACharacterBase::GetAvatar_Implementation()
{
	return this;
}

UAnimMontage* ACharacterBase::GetCombatMontage_Implementation()
{
	return CombatMontage;
}

ECharacterClass ACharacterBase::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

void ACharacterBase::MulticastHandleDeath_Implementation()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	bDead = true;
	
	OnDeath.Broadcast(this);
}

FOnAbilitySystemComponentRegistered& ACharacterBase::GetOnAbilitySystemComponentRegisteredDelegate()
{
	return OnAbilitySystemComponentRegistered;
}

FOnDeath& ACharacterBase::GetOnDeathDelegate()
{
	return OnDeath;
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
	CharacterAbilitySystemComponent->AddCharacterPassiveAbilities(StartPassiveAbilities);
}

FVector ACharacterBase::GetCombatSocketLocation_Implementation()
{
	check(GetMesh());
	return GetMesh()->GetSocketLocation(CombatSocketName);
}
