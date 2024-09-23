// Copyright DOGU0908


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelByExp(int32 Exp) const
{
	for (int i = 0; i < LevelUpInfo.Num(); ++i)
	{
		if (Exp < LevelUpInfo[i].LevelUpRequirement)
		{
			return i + 1;
		}
	}

	// return max level
	return LevelUpInfo.Num() + 1;
}
