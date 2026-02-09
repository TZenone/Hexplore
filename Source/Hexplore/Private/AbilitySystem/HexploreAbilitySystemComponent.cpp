// Copyright Tristan ZENONE.


#include "AbilitySystem/HexploreAbilitySystemComponent.h"

#include "AbilitySystem/Abilities/HexploreGameplayAbility.h"

void UHexploreAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UHexploreAbilitySystemComponent::EffectApplied);
	OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &UHexploreAbilitySystemComponent::EffectRemoved);
}

void UHexploreAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		
		if (const UHexploreGameplayAbility* HexploreAbility = Cast<UHexploreGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(HexploreAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
}

void UHexploreAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(Spec);
			
			if (!Spec.IsActive())
			{
				TryActivateAbility(Spec.Handle);
			}
		}
	}
}

void UHexploreAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(Spec);
		}
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
