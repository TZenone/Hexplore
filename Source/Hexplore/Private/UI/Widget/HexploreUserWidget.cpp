// Copyright Tristan ZENONE.


#include "UI/Widget/HexploreUserWidget.h"

void UHexploreUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
