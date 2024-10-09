// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnVolume.generated.h"

class AEnemySpawnPoint;
class UBoxComponent;

UCLASS()
class RUNESOFTHEARCHMAGE_API AEnemySpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:
	AEnemySpawnVolume();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditAnywhere)
	TArray<AEnemySpawnPoint*> SpawnPoints;
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> BoxComponent;
};
