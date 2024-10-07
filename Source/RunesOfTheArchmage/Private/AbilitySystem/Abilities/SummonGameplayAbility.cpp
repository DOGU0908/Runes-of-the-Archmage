// Copyright DOGU0908


#include "AbilitySystem/Abilities/SummonGameplayAbility.h"

#include "Actor/Storm.h"
#include "Kismet/KismetMathLibrary.h"
#include "RunesOfTheArchmage/RunesOfTheArchmage.h"

void USummonGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void USummonGameplayAbility::SpawnStorm(const FVector& TargetLocation)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority())
	{
		return;
	}

	FHitResult HitResult;
	const FVector TraceStartLocation = FVector(TargetLocation.X, TargetLocation.Y, TargetLocation.Z + 500.f);
	const FVector TraceEndLocation = FVector(TargetLocation.X, TargetLocation.Y, TargetLocation.Z - 500.f);
	
	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStartLocation, TraceEndLocation, ECC_Ground, FCollisionQueryParams::DefaultQueryParam))
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(HitResult.Location);
		SpawnTransform.SetRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal).Quaternion());
		
		if (AStorm* Storm = GetWorld()->SpawnActorDeferred<AStorm>(StormClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
		{
			Storm->DamageEffectParams = MakeDamageEffectParams();

			Storm->FinishSpawning(SpawnTransform);
		}
	}
}
