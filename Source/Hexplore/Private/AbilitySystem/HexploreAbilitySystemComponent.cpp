// Copyright Tristan ZENONE.


#include "AbilitySystem/HexploreAbilitySystemComponent.h"

void UHexploreAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UHexploreAbilitySystemComponent::EffectApplied);
	OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &UHexploreAbilitySystemComponent::EffectRemoved);
}

void UHexploreAbilitySystemComponent::AddCharacterAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		// GiveAbility(AbilitySpec);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UHexploreAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                    const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTagsAdded.Broadcast(TagContainer);

	for (const FGameplayTag& Tag : TagContainer)
	{
		// TODO: Broadcast the tag to the Widget Controller
		
	}
}

void UHexploreAbilitySystemComponent::EffectRemoved(const FActiveGameplayEffect& RemovedEffect)
{
	FGameplayTagContainer TagContainer;
	RemovedEffect.Spec.GetAllAssetTags(TagContainer);

	EffectAssetTagsRemoved.Broadcast(TagContainer);
}
