// Copyright Tristan ZENONE.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/HexploreAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UHexploreAttributeSet* AS = CastChecked<UHexploreAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(AS->GetHealth());
	OnMaxHealthChanged.Broadcast(AS->GetMaxHealth());
}
