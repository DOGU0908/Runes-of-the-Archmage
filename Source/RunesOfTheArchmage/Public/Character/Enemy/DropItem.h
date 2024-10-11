// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DropItem.generated.h"

USTRUCT(BlueprintType)
struct FDropItemInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> DropItemClass;

	UPROPERTY(EditAnywhere)
	float SpawnChance = 0.f;

	UPROPERTY(EditAnywhere)
	int32 MaxSpawnNumber = 0;
};

/**
 * 
 */
UCLASS()
class RUNESOFTHEARCHMAGE_API UDropItem : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	TArray<FDropItemInfo> GetDropItems();
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FDropItemInfo> DropItemInfos;
};
