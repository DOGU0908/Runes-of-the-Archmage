// Copyright DOGU0908


#include "Character/PlayerCharacterController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/CharacterAbilitySystemComponent.h"
#include "Input/CharacterInputComponent.h"
#include "Interaction/InteractionInterface.h"
#include "GameFramework/Character.h"
#include "UI/Widget/DamageTextComponent.h"

APlayerCharacterController::APlayerCharacterController()
{
	// updating data in server change data in clients
	bReplicates = true;
}

void APlayerCharacterController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	TraceCursor();
}

void APlayerCharacterController::ShowDamageNumber_Implementation(const float DamageAmount, ACharacter* TargetCharacter, const bool bIsCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass)
	{
		UDamageTextComponent* DamageTextComponent = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageTextComponent->RegisterComponent();

		DamageTextComponent->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageTextComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

		DamageTextComponent->SetDamageText(DamageAmount, bIsCriticalHit);
	}
}

void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();
	check(CharacterContext);

	/*
	 * input mapping
	 */
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	// subsystem may not exist in not locally controlled machines without valid local player
	if (Subsystem)
	{
		Subsystem->AddMappingContext(CharacterContext, 0);
	}

	/*
	 * mouse settings
	 */
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UCharacterInputComponent* CharacterInputComponent = CastChecked<UCharacterInputComponent>(InputComponent);

	CharacterInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::Move);
	CharacterInputComponent->BindAbilityActions(InputDataAsset, this, &APlayerCharacterController::AbilityInputTagPressed, &APlayerCharacterController::AbilityInputTagReleased, &APlayerCharacterController::AbilityInputTagHeld);
}

void APlayerCharacterController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);

	const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardVector, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightVector, InputAxisVector.X);
	}
}

void APlayerCharacterController::TraceCursor()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit)
	{
		return;
	}

	LastHoverActor = CurrentHoverActor;
	if (IsValid(CursorHit.GetActor()))
	{
		CurrentHoverActor = CursorHit.GetActor();
	}

	if (LastHoverActor == CurrentHoverActor)
	{
		return;
	}

	if (LastHoverActor)
	{
		LastHoverActor->UnHighlightActor();
	}
	if (CurrentHoverActor)
	{
		CurrentHoverActor->HighlightActor();
	}
}

void APlayerCharacterController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (!GetAbilitySystemComponent())
	{
		return;
	}

	GetAbilitySystemComponent()->AbilityPressedByInputTag(InputTag);
}

void APlayerCharacterController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!GetAbilitySystemComponent())
	{
		return;
	}

	GetAbilitySystemComponent()->AbilityReleasedByInputTag(InputTag);
}

void APlayerCharacterController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!GetAbilitySystemComponent())
	{
		return;
	}

	GetAbilitySystemComponent()->AbilityHeldByInputTag(InputTag);
}

UCharacterAbilitySystemComponent* APlayerCharacterController::GetAbilitySystemComponent()
{
	if (!CharacterAbilitySystemComponent)
	{
		CharacterAbilitySystemComponent = Cast<UCharacterAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	
	return CharacterAbilitySystemComponent;
}
