// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FLevelUpInfoData
{
	GENERATED_BODY()
	
	// exp required to pass this level, accumulative
	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointReward = 1;

	UPROPERTY(EditDefaultsOnly)
	int32 SpellPointReward = 1;
	
};

/**
 * 
 */
UCLASS()
class RUNESOFTHEARCHMAGE_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FLevelUpInfoData> LevelUpInfo;

	int32 FindLevelByExp(int32 Exp) const;
	
};
