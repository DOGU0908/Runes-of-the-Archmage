// Copyright DOGU0908


#include "Input/InputDataAsset.h"

const UInputAction* UInputDataAsset::FindAbilityInputActionByTag(const FGameplayTag& InputTag, const bool bLogNotFound) const
{
	for (const auto& InputActionData: AbilityInputActionData)
	{
		if (InputTag == InputActionData.InputTag && InputActionData.InputAction)
		{
			return InputActionData.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot find Ability Input Action"));
	}

	return nullptr;
}
