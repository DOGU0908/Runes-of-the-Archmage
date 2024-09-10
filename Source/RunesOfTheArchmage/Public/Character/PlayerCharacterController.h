// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

class UDamageTextComponent;
class UCharacterAbilitySystemComponent;
class UInputDataAsset;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IInteractionInterface;

/**
 * 
 */
UCLASS()
class RUNESOFTHEARCHMAGE_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerCharacterController();

	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter);

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> CharacterContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	void Move(const FInputActionValue& InputActionValue);

	void TraceCursor();
	TScriptInterface<IInteractionInterface> LastHoverActor;
	TScriptInterface<IInteractionInterface> CurrentHoverActor;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputDataAsset> InputDataAsset;
	
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY()
	TObjectPtr<UCharacterAbilitySystemComponent> CharacterAbilitySystemComponent;

	UCharacterAbilitySystemComponent* GetAbilitySystemComponent();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
};
