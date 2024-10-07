// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "MagicActor.h"
#include "Storm.generated.h"

class UBoxComponent;

UCLASS()
class RUNESOFTHEARCHMAGE_API AStorm : public AMagicActor
{
	GENERATED_BODY()
	
public:	
	AStorm();
	
protected:
	virtual void BeginPlay() override;
	virtual void LifeSpanExpired() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> RootSceneComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> BoxComponent;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void DealDamage();

private:
	FTimerHandle DamageTimerHandle;
	
	TArray<TWeakObjectPtr<AActor>> OverlappingActors;
};
