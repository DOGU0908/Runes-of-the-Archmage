// Copyright DOGU0908


#include "Character/PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/CharacterAbilitySystemComponent.h"
#include "Character/PlayerCharacterController.h"
#include "Character/PlayerCharacterState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/HUD/HUDBase.h"

APlayerCharacter::APlayerCharacter()
{
	/*
	 * weapon mesh
	 */
	Weapon = CreateDefaultSubobject<UStaticMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	/*
	 * character rotation
	 */
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	// top down character movement constrain to plane normal vector
	GetCharacterMovement()->bConstrainToPlane = true;
	// character initial position on the plane
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// server init ability actor info
	InitAbilityActorInfo();
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// client init ability actor info
	InitAbilityActorInfo();
}

/*
 * Player Controlled Character
 * 1. When Ability System Component is inside the Pawn
 * - Server: PossessedBy
 * - Client: AcknowledgePossession
 * 2. When Ability System Component is inside Player State
 * - Server: PossessedBy
 * - Client: OnRep_PlayerState
 *
 * AI Controlled Character
 * - Server: BeginPlay
 * - Client: BeginPlay
 */

void APlayerCharacter::InitAbilityActorInfo()
{
	APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);
	PlayerCharacterState->GetAbilitySystemComponent()->InitAbilityActorInfo(PlayerCharacterState, this);
	Cast<UCharacterAbilitySystemComponent>(PlayerCharacterState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = PlayerCharacterState->GetAbilitySystemComponent();
	AttributeSet = PlayerCharacterState->GetAttributeSet();

	// all 4 key variables to init hud is ensured to be initialized
	// clients do not have valid player controllers for other characters
	if (APlayerCharacterController* PlayerCharacterController = Cast<APlayerCharacterController>(GetController()))
	{
		if (AHUDBase* HUDBase = Cast<AHUDBase>(PlayerCharacterController->GetHUD()))
		{
			HUDBase->InitOverlay(PlayerCharacterController, PlayerCharacterState, AbilitySystemComponent, AttributeSet);
		}
	}
}
