// Copyright DOGU0908


#include "Actor/EnemySpawnVolume.h"

#include "Actor/EnemySpawnPoint.h"
#include "Character/PlayerInterface.h"
#include "Components/BoxComponent.h"

AEnemySpawnVolume::AEnemySpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionObjectType(ECC_WorldStatic);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AEnemySpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemySpawnVolume::OnBoxOverlap);
}

void AEnemySpawnVolume::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->Implements<UPlayerInterface>())
	{
		return;
	}

	for (const AEnemySpawnPoint* SpawnPoint: SpawnPoints)
	{
		if (IsValid(SpawnPoint))
		{
			SpawnPoint->SpawnEnemy();
		}
	}

	// enable the following to set the spawn volume one time fire, or create variable to control this if needed
	// BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
