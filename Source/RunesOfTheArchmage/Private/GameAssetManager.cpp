// Copyright DOGU0908


#include "GameAssetManager.h"

#include "GameplayTagSingleton.h"

UGameAssetManager& UGameAssetManager::Get()
{
	check(GEngine);
	
	UGameAssetManager* GameAssetManager = Cast<UGameAssetManager>(GEngine->AssetManager);
	return *GameAssetManager;
}

void UGameAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FGameplayTagSingleton::InitializeNativeGameplayTags();
}
