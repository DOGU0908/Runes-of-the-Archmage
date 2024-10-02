// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "PlayerInterface.h"
#include "Character/CharacterBase.h"
#include "PlayerCharacter.generated.h"

class UNiagaraComponent;

/**
 * 
 */
UCLASS()
class RUNESOFTHEARCHMAGE_API APlayerCharacter : public ACharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual int32 GetCharacterLevel() override;

	virtual void Die() override;

	virtual void MulticastHandleDeath_Implementation() override;

	virtual int32 GetExp_Implementation() const override;
	virtual void AddExp_Implementation(int32 InExp) override;
	
	virtual void LevelUp_Implementation() override;
	virtual int32 FindLevelByExp_Implementation(int32 InExp) const override;
	virtual void AddPlayerLevel_Implementation(int32 InPlayerLevel) override;
	
	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;
	virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override;
	virtual void AddAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual void AddSpellPoints_Implementation(int32 InSpellPoints) override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetSpellPoints_Implementation() const override;

	virtual void OnRep_Burn() const override;
	virtual void OnRep_Shock() const override;
	virtual void OnRep_Frozen() override;

protected:
	virtual void InitAbilityActorInfo() override;

	virtual FVector GetCombatSocketLocation_Implementation() override;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultBaseAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultBaseVitalAttributes;

	virtual void InitializeDefaultAttributes() const override;
	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const float Level) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

private:
	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<UStaticMeshComponent> Weapon;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;
	
};
