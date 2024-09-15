// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/CharacterBase.h"
#include "Combat/EnemyInterface.h"
#include "Interaction/InteractionInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Enemy.generated.h"

class AEnemyAIController;
class UBehaviorTree;
enum class ECharacterClass : uint8;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class RUNESOFTHEARCHMAGE_API AEnemy : public ACharacterBase, public IInteractionInterface, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void PossessedBy(AController* NewController) override;
	
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	virtual int32 GetCharacterLevel() override;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnMaxHealthChanged;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category="Hit")
	bool bHitReacting = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement")
	float BaseWalkSpeed = 250.f;

	virtual void Die() override;

	virtual void MulticastHandleDeath_Implementation() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	float LifeSpan = 5.f;

	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() override;
	
protected:
	virtual void BeginPlay() override;
	
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Melee;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(EditAnywhere, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AEnemyAIController> AIController;

	UPROPERTY(BlueprintReadWrite, Category="Combat")
	TObjectPtr<AActor> CombatTarget;
	
};
