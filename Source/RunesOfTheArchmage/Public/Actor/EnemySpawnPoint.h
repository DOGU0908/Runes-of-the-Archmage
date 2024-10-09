// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "EnemySpawnPoint.generated.h"

class AEnemy;
/**
 * 
 */
UCLASS()
class RUNESOFTHEARCHMAGE_API AEnemySpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy() const;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AEnemy> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 EnemyLevel = 1;
};
