// Copyright DOGU0908


#include "Character/PlayerCharacter.h"

APlayerCharacter::APlayerCharacter()
{
	Weapon = CreateDefaultSubobject<UStaticMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
