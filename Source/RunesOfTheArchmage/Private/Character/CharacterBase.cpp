// Copyright DOGU0908


#include "Character/CharacterBase.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagSingleton.h"
#include "AbilitySystem/CharacterAbilitySystemComponent.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "RunesOfTheArchmage/RunesOfTheArchmage.h"

ACharacterBase::ACharacterBase()
{
 	PrimaryActorTick.bCanEverTick = false;

	BurnDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("BurnDebuffComponent");
	BurnDebuffComponent->SetupAttachment(GetRootComponent());
	BurnDebuffComponent->DebuffTag = FGameplayTagSingleton::Get().DebuffBurn;

	ShockDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("ShockDebuffComponent");
	ShockDebuffComponent->SetupAttachment(GetRootComponent());
	ShockDebuffComponent->DebuffTag = FGameplayTagSingleton::Get().DebuffShock;

	FreezeDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("FreezeDebuffComponent");
	FreezeDebuffComponent->SetupAttachment(GetRootComponent());
	FreezeDebuffComponent->DebuffTag = FGameplayTagSingleton::Get().DebuffFreeze;
	
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
}

void ACharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACharacterBase, bIsBurn);
	DOREPLIFETIME(ACharacterBase, bIsShock);
	DOREPLIFETIME(ACharacterBase, bIsFrozen);
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
	BurnDebuffComponent->Deactivate();
	ShockDebuffComponent->Deactivate();
	FreezeDebuffComponent->Deactivate();
	
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	bDead = true;
}

FOnAbilitySystemComponentRegistered& ACharacterBase::GetOnAbilitySystemComponentRegisteredDelegate()
{
	return OnAbilitySystemComponentRegistered;
}

void ACharacterBase::BurnTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsBurn = NewCount > 0;
}

void ACharacterBase::ShockTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsShock = NewCount > 0;
}

void ACharacterBase::FreezeTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsFrozen = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsFrozen ? 0.f : BaseWalkSpeed;
}

void ACharacterBase::OnRep_Burn() const
{
}

void ACharacterBase::OnRep_Shock() const
{
}

void ACharacterBase::OnRep_Frozen()
{
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
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
