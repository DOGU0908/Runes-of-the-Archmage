// Copyright DOGU0908


#include "UI/Widget/UserWidgetBase.h"

void UUserWidgetBase::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}
