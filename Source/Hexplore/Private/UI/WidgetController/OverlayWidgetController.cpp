// Copyright Tristan ZENONE.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/HexploreAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UHexploreAttributeSet* AS = CastChecked<UHexploreAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(AS->GetHealth());
	OnMaxHealthChanged.Broadcast(AS->GetMaxHealth());
	OnStaminaChanged.Broadcast(AS->GetStamina());
	OnMaxStaminaChanged.Broadcast(AS->GetMaxStamina());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UHexploreAttributeSet* AS = CastChecked<UHexploreAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetStaminaAttribute()).AddUObject(this, &UOverlayWidgetController::StaminaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxStaminaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxStaminaChanged);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::StaminaChanged(const FOnAttributeChangeData& Data) const
{
	OnStaminaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxStaminaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxStaminaChanged.Broadcast(Data.NewValue);
}
