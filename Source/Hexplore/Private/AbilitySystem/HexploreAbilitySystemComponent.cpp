// Copyright Tristan ZENONE.


#include "AbilitySystem/HexploreAbilitySystemComponent.h"

void UHexploreAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UHexploreAbilitySystemComponent::EffectApplied);
}

void UHexploreAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                    const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);

	for (const FGameplayTag& Tag : TagContainer)
	{
		// TODO: Broadcast the tag to the Widget Controller
		
	}
}
