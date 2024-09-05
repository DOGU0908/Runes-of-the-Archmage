// Copyright DOGU0908


#include "AbilitySystem/Data/AttributeInfo.h"

FBaseAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FBaseAttributeInfo& Info: AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot find Attribute Tag [%s]."), *AttributeTag.ToString());
	}

	return FBaseAttributeInfo();
}
