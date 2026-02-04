// Copyright Tristan ZENONE.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/HexploreAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UHexploreAttributeSet* AS = CastChecked<UHexploreAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(AS->GetHealth());
	OnMaxHealthChanged.Broadcast(AS->GetMaxHealth());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UHexploreAttributeSet* AS = CastChecked<UHexploreAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}
