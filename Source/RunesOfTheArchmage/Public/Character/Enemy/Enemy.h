// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Interaction/InteractionInterface.h"
#include "Enemy.generated.h"

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

protected:
	virtual void BeginPlay() override;
	
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	int32 Level = 1;
	
};
