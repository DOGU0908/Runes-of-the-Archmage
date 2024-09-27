// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Combat/CombatInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class UDebuffNiagaraComponent;
class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)
class RUNESOFTHEARCHMAGE_API ACharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const;

	virtual UAnimMontage* GetHitReactMontage_Implementation() override;

	virtual void Die() override;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

	virtual bool IsDead_Implementation() const override;
	
	virtual AActor* GetAvatar_Implementation() override;

	virtual UAnimMontage* GetCombatMontage_Implementation() override;

	virtual ECharacterClass GetCharacterClass_Implementation() override;

	FOnAbilitySystemComponentRegistered OnAbilitySystemComponentRegistered;
	virtual FOnAbilitySystemComponentRegistered& GetOnAbilitySystemComponentRegisteredDelegate() override;

	FOnDeath OnDeath;
	virtual FOnDeath& GetOnDeathDelegate() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	virtual void InitAbilityActorInfo();
	
	virtual void InitializeDefaultAttributes() const;

	void AddCharacterAbilities() const;

	UPROPERTY(EditAnywhere, Category="Combat")
	FName CombatSocketName;

	virtual FVector GetCombatSocketLocation_Implementation() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Melee;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> BurnDebuffComponent;

private:
	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartAbilities;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartPassiveAbilities;

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
	
	bool bDead = false;

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<UAnimMontage> CombatMontage;
	
};
