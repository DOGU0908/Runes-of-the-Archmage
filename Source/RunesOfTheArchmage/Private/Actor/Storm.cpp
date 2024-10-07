// Copyright DOGU0908


#include "Actor/Storm.h"

#include "Components/BoxComponent.h"

AStorm::AStorm()
{
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>("RootSceneComponent");
	SetRootComponent(RootSceneComponent);
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AStorm::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AStorm::OnBoxOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AStorm::AStorm::OnBoxEndOverlap);

	GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &AStorm::DealDamage, 1.0f, true);
}

void AStorm::LifeSpanExpired()
{
	// clear timer
	GetWorldTimerManager().ClearTimer(DamageTimerHandle);
	
	Super::LifeSpanExpired();
}

void AStorm::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                          int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority())
	{
		return;
	}
	
	if (ShouldCauseDamage(OtherActor))
	{
		OverlappingActors.AddUnique(OtherActor);
	}
}

void AStorm::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!HasAuthority())
	{
		return;
	}
	
	if (OverlappingActors.Contains(OtherActor))
	{
		OverlappingActors.Remove(OtherActor);
	}
}

void AStorm::DealDamage()
{
	if (!HasAuthority())
	{
		return;
	}
		
	OverlappingActors.RemoveAll(
		[](const TWeakObjectPtr<AActor>& Actor)
		{
			return !Actor.IsValid();
		}
	);

	TArray<TWeakObjectPtr<AActor>> CopiedOverlappingActors = OverlappingActors;
	for (auto Actor: CopiedOverlappingActors)
	{
		if (Actor.IsValid() && ShouldCauseDamage(Actor.Get()))
		{
			Multicast_ShowHitEffect(Actor->GetActorLocation(), Actor->GetActorRotation());
			
			ApplyDamage(Actor.Get());
		}
	}
}
