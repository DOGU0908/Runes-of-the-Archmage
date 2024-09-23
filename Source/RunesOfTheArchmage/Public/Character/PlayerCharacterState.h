// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "PlayerCharacterState.generated.h"

class ULevelUpInfo;
class UAbilitySystemComponent;
class UAttributeSet;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChange, int32)

/**
 * 
 */
UCLASS()
class RUNESOFTHEARCHMAGE_API APlayerCharacterState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	APlayerCharacterState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FORCEINLINE int32 GetCharacterLevel() const {return Level;}
	void AddLevel(const int32 InLevel);
	void SetLevel(const int32 InLevel);

	FOnPlayerStatChange OnLevelChangeDelegate;
	
	FORCEINLINE int32 GetExp() const {return Exp;}
	void AddToExp(const int32 InExp);
	void SetExp(const int32 InExp);
	
	FOnPlayerStatChange OnExpChangeDelegate;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level = 1;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel) const;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Exp)
	int32 Exp = 0;

	UFUNCTION()
	void OnRep_Exp(int32 OldExp) const;
	
};
