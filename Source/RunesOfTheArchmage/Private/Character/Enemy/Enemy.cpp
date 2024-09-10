// Copyright DOGU0908


#include "Character/Enemy/Enemy.h"

#include "GameplayTagSingleton.h"
#include "AbilitySystem/AbilitySystemLibrary.h"
#include "AbilitySystem/CharacterAbilitySystemComponent.h"
#include "AbilitySystem/CharacterAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RunesOfTheArchmage/RunesOfTheArchmage.h"
#include "UI/Widget/UserWidgetBase.h"

AEnemy::AEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);

	AbilitySystemComponent = CreateDefaultSubobject<UCharacterAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
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
}

void AEnemy::Die()
{
	SetLifeSpan(LifeSpan);
	Super::Die();
}

void AEnemy::MulticastHandleDeath_Implementation()
{
	Super::MulticastHandleDeath_Implementation();

	HealthBar->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	InitAbilityActorInfo();
	UAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent);

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
	
	InitializeDefaultAttributes();
}

void AEnemy::InitializeDefaultAttributes() const
{
	UAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}
