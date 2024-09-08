// Copyright DOGU0908


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* Object = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	
	return Object;
}

void UTargetDataUnderMouse::Activate()
{
	if (Ability->GetCurrentActorInfo()->IsLocallyControlled())
	{
		SendMouseCursorData();
	}
	else
	{
		// listen for target data
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddLambda(
			[this, SpecHandle, ActivationPredictionKey](const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
			{
				// called in response to receiving target data (replicated)
				AbilitySystemComponent->ConsumeClientReplicatedTargetData(SpecHandle, ActivationPredictionKey);

				if (ShouldBroadcastAbilityTaskDelegates())
				{
					ValidData.Broadcast(DataHandle);
				}
			}
		);

		// broadcast target data delegate if already received (available)
		if (!AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey))
		{
			// trigger the bind lambda function to be called when target data arrives
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	FScopedPredictionWindow ScopedPredictionWindow(AbilitySystemComponent.Get());
	
	FHitResult HitResult;
	Ability->GetCurrentActorInfo()->PlayerController.Get()->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	
	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = HitResult;
	DataHandle.Add(Data);
	
	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey(), DataHandle, FGameplayTag(), AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
