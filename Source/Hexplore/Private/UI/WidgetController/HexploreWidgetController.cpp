// Copyright Tristan ZENONE.


#include "UI/WidgetController/HexploreWidgetController.h"

void UHexploreWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UHexploreWidgetController::BroadcastInitialValues()
{
	
}

void UHexploreWidgetController::BindCallbacksToDependencies()
{
	
}
