// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "InputDataAsset.h"
#include "CharacterInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class RUNESOFTHEARCHMAGE_API UCharacterInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UInputDataAsset* InputDataAsset, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
	
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UCharacterInputComponent::BindAbilityActions(const UInputDataAsset* InputDataAsset, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputDataAsset);

	for (const auto& ActionData: InputDataAsset->AbilityInputActionData)
	{
		if (ActionData.InputAction && ActionData.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(ActionData.InputAction, ETriggerEvent::Started, Object, PressedFunc, ActionData.InputTag);
			}

			if (ReleasedFunc)
			{
				BindAction(ActionData.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, ActionData.InputTag);
			}
			
			if (HeldFunc)
			{
				BindAction(ActionData.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, ActionData.InputTag);
			}
		}
	}
}
