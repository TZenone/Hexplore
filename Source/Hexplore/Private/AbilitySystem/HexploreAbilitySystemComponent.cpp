// Copyright Tristan ZENONE.


#include "AbilitySystem/HexploreAbilitySystemComponent.h"

void UHexploreAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UHexploreAbilitySystemComponent::EffectApplied);
}

void UHexploreAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                    const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	
}
