// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/CharacterBase.h"
#include "Interaction/InteractionInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Enemy.generated.h"

enum class ECharacterClass : uint8;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class RUNESOFTHEARCHMAGE_API AEnemy : public ACharacterBase, public IInteractionInterface
{
	GENERATED_BODY()

public:
	AEnemy();
	
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

	UPROPERTY(BlueprintReadOnly, Category="Movement")
	float BaseWalkSpeed = 250.f;

	virtual void Die() override;

	virtual void MulticastHandleDeath_Implementation() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	float LifeSpan = 5.f;
	
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
	
};
