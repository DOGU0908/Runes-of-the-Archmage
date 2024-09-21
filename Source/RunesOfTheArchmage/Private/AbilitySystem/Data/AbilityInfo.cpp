// Copyright DOGU0908


#include "AbilitySystem/Data/AbilityInfo.h"

FAbilityInfoData UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const auto& Info: AbilityInfo)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("No Ability found"));
	}

	return FAbilityInfoData();
}
