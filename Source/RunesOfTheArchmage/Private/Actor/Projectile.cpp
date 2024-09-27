// Copyright DOGU0908


#include "Actor/Projectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AbilitySystemLibrary.h"
#include "RunesOfTheArchmage/RunesOfTheArchmage.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(SphereComponent);
	SphereComponent->SetCollisionObjectType(ECC_Projectile);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeSpan);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnSphereOverlap);
	
	// can use UGameplayStatics::SpawnSoundAttached which returns an UAudioComponent for looping sound
}

void AProjectile::Destroyed()
{
	// in case the actor is destroyed on the server before overlapped on the client
	if (!bHit && !HasAuthority())
	{
		OnHit();
	}
	
	Super::Destroyed();
}

void AProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	// prevent the shooter to generate sphere overlap event
	// if the enemy also has a projectile ability, this is important
	if (SourceAvatarActor == OtherActor)
	{
		return;
	}

	// prevent friendly unit attack by projectile
	if (!UAbilitySystemLibrary::IsNotFriendlyUnit(SourceAvatarActor, OtherActor))
	{
		return;
	}

	// bugfix: clients seems to play sound twice (larger sound)
	if (!bHit)
	{
		OnHit();
	}

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			// set target ability system component here (when projectile hits)
			DamageEffectParams.TargetAbilitySystemComponent = TargetAbilitySystemComponent;
			UAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
		
		Destroy();
	}
	else
	{
		bHit = true;
	}
}

void AProjectile::OnHit()
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation(), GetActorRotation());
	bHit = true;
}
