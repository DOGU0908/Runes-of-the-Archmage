// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class RUNESOFTHEARCHMAGE_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual int32 GetCharacterLevel() override;

	virtual void Die() override;

	virtual void MulticastHandleDeath_Implementation() override;

protected:
	virtual void InitAbilityActorInfo() override;

	virtual FVector GetCombatSocketLocation() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultBaseAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultBaseVitalAttributes;

	virtual void InitializeDefaultAttributes() const override;
	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const float Level) const;

private:
	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<UStaticMeshComponent> Weapon;
	
};
