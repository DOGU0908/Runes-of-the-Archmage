// Copyright DOGU0908


#include "UI/WidgetController/WidgetControllerBase.h"

void UWidgetControllerBase::SetWidgetControllerParams(const FWidgetControllerParams& WidgetControllerParams)
{
	PlayerController = WidgetControllerParams.PlayerController;
	PlayerState = WidgetControllerParams.PlayerState;
	AbilitySystemComponent = WidgetControllerParams.AbilitySystemComponent;
	AttributeSet = WidgetControllerParams.AttributeSet;
}

void UWidgetControllerBase::BroadcastInitialValues()
{
}

void UWidgetControllerBase::BindCallbacksToDependencies()
{
}
