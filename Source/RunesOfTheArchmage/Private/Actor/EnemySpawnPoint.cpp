// Copyright DOGU0908


#include "Actor/EnemySpawnPoint.h"

#include "Character/Enemy/Enemy.h"

void AEnemySpawnPoint::SpawnEnemy() const
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	if (UWorld* World = GetWorld())
	{
		AEnemy* Enemy = World->SpawnActorDeferred<AEnemy>(EnemyClass, GetActorTransform());

		Enemy->SetLevel(EnemyLevel);
		
		Enemy->FinishSpawning(GetActorTransform());
		Enemy->SpawnDefaultController();
	}
}
