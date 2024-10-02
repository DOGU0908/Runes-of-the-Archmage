// Copyright DOGU0908


#include "Character/PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagSingleton.h"
#include "AbilitySystem/CharacterAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Character/PlayerCharacterController.h"
#include "Character/PlayerCharacterState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/HUD/HUDBase.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"

APlayerCharacter::APlayerCharacter()
{
	/*
	 * weapon mesh
	 */
	Weapon = CreateDefaultSubobject<UStaticMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	/*
	 * character rotation
	 */
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	// top down character movement constrain to plane normal vector
	GetCharacterMovement()->bConstrainToPlane = true;
	// character initial position on the plane
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	/*
	 * Character Class
	 */
	CharacterClass = ECharacterClass::Wizard;

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// server init ability actor info
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// client init ability actor info
	InitAbilityActorInfo();
}

int32 APlayerCharacter::GetCharacterLevel()
{
	const APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);

	return PlayerCharacterState->GetCharacterLevel();
}

void APlayerCharacter::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	Super::Die();
}

void APlayerCharacter::MulticastHandleDeath_Implementation()
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	
	Super::MulticastHandleDeath_Implementation();
}

int32 APlayerCharacter::GetExp_Implementation() const
{
	const APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);

	return PlayerCharacterState->GetExp();
}

void APlayerCharacter::AddExp_Implementation(int32 InExp)
{
	APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);

	PlayerCharacterState->AddToExp(InExp);
}

void APlayerCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

int32 APlayerCharacter::FindLevelByExp_Implementation(int32 InExp) const
{
	const APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);

	return PlayerCharacterState->LevelUpInfo->FindLevelByExp(InExp);
}

void APlayerCharacter::AddPlayerLevel_Implementation(int32 InPlayerLevel)
{
	APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);

	PlayerCharacterState->AddLevel(InPlayerLevel);

	if (UCharacterAbilitySystemComponent* CharacterAbilitySystemComponent = Cast<UCharacterAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		CharacterAbilitySystemComponent->UpdateAbilityStatus(PlayerCharacterState->GetCharacterLevel());
	}
}

int32 APlayerCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	const APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);

	return PlayerCharacterState->LevelUpInfo->LevelUpInfo[Level].AttributePointReward;
}

int32 APlayerCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	const APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);

	return PlayerCharacterState->LevelUpInfo->LevelUpInfo[Level].SpellPointReward;
}

void APlayerCharacter::AddAttributePoints_Implementation(int32 InAttributePoints)
{
	APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);

	PlayerCharacterState->AddAttributePoints(InAttributePoints);
}

void APlayerCharacter::AddSpellPoints_Implementation(int32 InSpellPoints)
{
	APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);

	PlayerCharacterState->AddSpellPoints(InSpellPoints);
}

int32 APlayerCharacter::GetAttributePoints_Implementation() const
{
	const APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);

	return PlayerCharacterState->GetAttributePoints();
}

int32 APlayerCharacter::GetSpellPoints_Implementation() const
{
	const APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);

	return PlayerCharacterState->GetSpellPoints();
}

void APlayerCharacter::OnRep_Burn() const
{
	if (bIsBurn)
	{
		BurnDebuffComponent->Activate();
	}
	else
	{
		BurnDebuffComponent->Deactivate();
	}
}

void APlayerCharacter::OnRep_Shock() const
{
	if (bIsShock)
	{
		ShockDebuffComponent->Activate();
	}
	else
	{
		ShockDebuffComponent->Deactivate();
	}
}

void APlayerCharacter::OnRep_Frozen()
{
	Super::OnRep_Frozen();
	
	if (UCharacterAbilitySystemComponent* CharacterAbilitySystemComponent = Cast<UCharacterAbilitySystemComponent>(AbilitySystemComponent))
	{
		FGameplayTagContainer BlockedTags;
		BlockedTags.AddTag(FGameplayTagSingleton::Get().Player_Block_AbilityActivation);

		if (bIsFrozen)
		{
			CharacterAbilitySystemComponent->AddLooseGameplayTags(BlockedTags);
			FreezeDebuffComponent->Activate();
		}
		else
		{
			CharacterAbilitySystemComponent->RemoveLooseGameplayTags(BlockedTags);
			FreezeDebuffComponent->Deactivate();
		}
	}
}

/*
 * Player Controlled Character
 * 1. When Ability System Component is inside the Pawn
 * - Server: PossessedBy
 * - Client: AcknowledgePossession
 * 2. When Ability System Component is inside Player State
 * - Server: PossessedBy
 * - Client: OnRep_PlayerState
 *
 * AI Controlled Character
 * - Server: BeginPlay
 * - Client: BeginPlay
 */

void APlayerCharacter::InitAbilityActorInfo()
{
	APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);
	PlayerCharacterState->GetAbilitySystemComponent()->InitAbilityActorInfo(PlayerCharacterState, this);
	Cast<UCharacterAbilitySystemComponent>(PlayerCharacterState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = PlayerCharacterState->GetAbilitySystemComponent();
	AttributeSet = PlayerCharacterState->GetAttributeSet();
	OnAbilitySystemComponentRegistered.Broadcast(AbilitySystemComponent);
	
	const FGameplayTagSingleton& GameplayTags = FGameplayTagSingleton::Get();
	AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTags.DebuffBurn, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &APlayerCharacter::BurnTagChanged);
	AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTags.DebuffShock, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &APlayerCharacter::ShockTagChanged);
	AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTags.DebuffFreeze, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &APlayerCharacter::FreezeTagChanged);
	
	// all 4 key variables to init hud is ensured to be initialized
	// clients do not have valid player controllers for other characters
	if (APlayerCharacterController* PlayerCharacterController = Cast<APlayerCharacterController>(GetController()))
	{
		if (AHUDBase* HUDBase = Cast<AHUDBase>(PlayerCharacterController->GetHUD()))
		{
			HUDBase->InitOverlay(PlayerCharacterController, PlayerCharacterState, AbilitySystemComponent, AttributeSet);
		}
	}

	// it is allowable to call only in server, client will replicate the attributes
	InitializeDefaultAttributes();
}

FVector APlayerCharacter::GetCombatSocketLocation_Implementation()
{
	check(Weapon);
	return Weapon->GetSocketLocation(CombatSocketName);
}

void APlayerCharacter::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultBaseAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	// set health and mana after setting max health and max mana based on base attributes
	ApplyEffectToSelf(DefaultBaseVitalAttributes, 1.f);
}

void APlayerCharacter::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	
	FGameplayEffectContextHandle GameplayEffectContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	GameplayEffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle GameplayEffectSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, GameplayEffectContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());
}

void APlayerCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		LevelUpNiagaraComponent->Activate(true);
	}
}
