// Copyright DOGU0908


#include "Character/Enemy/Enemy.h"

#include "GameplayTagSingleton.h"
#include "AbilitySystem/AbilitySystemLibrary.h"
#include "AbilitySystem/CharacterAbilitySystemComponent.h"
#include "AbilitySystem/CharacterAttributeSet.h"
#include "AI/EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RunesOfTheArchmage/RunesOfTheArchmage.h"
#include "UI/Widget/UserWidgetBase.h"

AEnemy::AEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UCharacterAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	
	AttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());

	BaseWalkSpeed = 250.f;
}

void AEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// only set AI controller on the server
	// clients get replicated data
	if (!HasAuthority())
	{
		return;
	}
	
	AIController = Cast<AEnemyAIController>(NewController);
	AIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	AIController->RunBehaviorTree(BehaviorTree);
	AIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	AIController->GetBlackboardComponent()->SetValueAsBool(FName("IsRanged"), CharacterClass != ECharacterClass::Melee);
}

void AEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_INTERACTION);
}

void AEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
}

int32 AEnemy::GetCharacterLevel()
{
	return Level;
}

void AEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;

	// no movement while hit react
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	if (AIController && AIController->GetBlackboardComponent())
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}
}

void AEnemy::Die()
{
	SetLifeSpan(LifeSpan);

	// preventing attack while the enemy is dead but not destroyed
	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	}
	
	Super::Die();
}

void AEnemy::MulticastHandleDeath_Implementation()
{
	Super::MulticastHandleDeath_Implementation();

	HealthBar->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}

void AEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* AEnemy::GetCombatTarget_Implementation()
{
	return CombatTarget;
}

void AEnemy::FreezeTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::FreezeTagChanged(CallbackTag, NewCount);

	// no movement while frozen
	if (AIController && AIController->GetBlackboardComponent())
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(FName("Frozen"), bIsFrozen);
	}
}

void AEnemy::SetLevel(const int32 InLevel)
{
	Level = InLevel;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	// initialize abilities and attributes only on server for the enemy
	InitAbilityActorInfo();
	if (HasAuthority())
	{
		UAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}

	if (UUserWidgetBase* Widget = Cast<UUserWidgetBase>(HealthBar->GetUserWidgetObject()))
	{
		Widget->SetWidgetController(this);
	}
	
	if (const UCharacterAttributeSet* CharacterAttributeSet = CastChecked<UCharacterAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CharacterAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CharacterAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTagSingleton::Get().EffectsHitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AEnemy::HitReactTagChanged);

		OnHealthChanged.Broadcast(CharacterAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(CharacterAttributeSet->GetMaxHealth());
	}
}

void AEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UCharacterAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTagSingleton::Get().DebuffFreeze, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AEnemy::FreezeTagChanged);

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
	OnAbilitySystemComponentRegistered.Broadcast(AbilitySystemComponent);
}

void AEnemy::InitializeDefaultAttributes() const
{
	UAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}
