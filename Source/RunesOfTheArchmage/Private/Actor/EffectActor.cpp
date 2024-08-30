// Copyright DOGU0908


#include "Actor/EffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/CharacterAttributeSet.h"
#include "Components/SphereComponent.h"

AEffectActor::AEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
}

void AEffectActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// TODO: change to gameplay effect
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UCharacterAttributeSet* CharacterAttributeSet = Cast<UCharacterAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UCharacterAttributeSet::StaticClass()));
		UCharacterAttributeSet* MutableCharacterAttributeSet = const_cast<UCharacterAttributeSet*>(CharacterAttributeSet);
		MutableCharacterAttributeSet->SetHealth(CharacterAttributeSet->GetHealth() + 10.f);
		MutableCharacterAttributeSet->SetMana(CharacterAttributeSet->GetMana() + 10.f);
		Destroy();
	}
}

void AEffectActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AEffectActor::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AEffectActor::OnBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AEffectActor::OnEndOverlap);
}
