// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RunesOfTheArchmageGameModeBase.generated.h"

class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class RUNESOFTHEARCHMAGE_API ARunesOfTheArchmageGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
	
};
