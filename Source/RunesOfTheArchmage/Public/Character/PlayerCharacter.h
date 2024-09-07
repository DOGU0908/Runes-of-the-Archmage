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

protected:
	virtual void InitAbilityActorInfo() override;

	virtual FVector GetCombatSocketLocation() override;
	
private:
	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<UStaticMeshComponent> Weapon;
	
};
