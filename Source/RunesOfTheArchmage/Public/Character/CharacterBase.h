// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Combat/CombatInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

struct FGameplayTag;
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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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

	virtual void BurnTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	virtual void ShockTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	// freeze tag is required to stop enemy behavior tree, so it should be bound to enemy ability system component gameplay tag event
	virtual void FreezeTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement")
	float BaseWalkSpeed = 600.f;

	// debuff effects not replicated for player character
	// this is mainly because only enemies are replicating ability system component
	// use boolean variables to set the state of debuff to display niagara effect
	UPROPERTY(ReplicatedUsing=OnRep_Burn, BlueprintReadOnly)
	bool bIsBurn = false;

	UFUNCTION()
	virtual void OnRep_Burn() const;

	UPROPERTY(ReplicatedUsing=OnRep_Shock, BlueprintReadOnly)
	bool bIsShock = false;

	UFUNCTION()
	virtual void OnRep_Shock() const;
	
	UPROPERTY(ReplicatedUsing=OnRep_Frozen, BlueprintReadOnly)
	bool bIsFrozen = false;

	UFUNCTION()
	virtual void OnRep_Frozen();

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

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> ShockDebuffComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> FreezeDebuffComponent;

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
