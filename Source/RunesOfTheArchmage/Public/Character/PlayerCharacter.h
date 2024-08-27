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

private:
	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<UStaticMeshComponent> Weapon;
	
};
