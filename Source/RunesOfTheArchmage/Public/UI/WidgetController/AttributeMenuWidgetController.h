// Copyright DOGU0908

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/WidgetControllerBase.h"
#include "AttributeMenuWidgetController.generated.h"

struct FGameplayAttribute;
struct FGameplayTag;
class UAttributeInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoDelegate, const FBaseAttributeInfo&, Info);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class RUNESOFTHEARCHMAGE_API UAttributeMenuWidgetController : public UWidgetControllerBase
{
	GENERATED_BODY()

public:
	virtual void BindCallbacksToDependencies() override;

	virtual void BroadcastInitialValues() override;

	UPROPERTY(BlueprintAssignable)
	FAttributeInfoDelegate AttributeInfoDelegate;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& GameplayAttribute) const;
	
};
