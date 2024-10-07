// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "AbilityTypes.h"
#include "GameFramework/Actor.h"
#include "MagicActor.generated.h"

class UNiagaraSystem;

UCLASS()
class RUNESOFTHEARCHMAGE_API AMagicActor : public AActor
{
	GENERATED_BODY()
	
public:
	AMagicActor();

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn=true))
	FDamageEffectParams DamageEffectParams;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 5.f;

	bool ShouldCauseDamage(const AActor* OtherActor) const;

	void ApplyDamage(AActor* OtherActor);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ShowHitEffect(const FVector& SpawnLocation, const FRotator& SpawnRotation) const;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

};
