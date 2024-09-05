// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "GameAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class RUNESOFTHEARCHMAGE_API UGameAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UGameAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
	
};
