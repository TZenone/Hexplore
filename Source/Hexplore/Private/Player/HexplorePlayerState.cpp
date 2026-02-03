// Copyright Tristan ZENONE.


#include "Player/HexplorePlayerState.h"

#include "AbilitySystem/HexploreAbilitySystemComponent.h"
#include "AbilitySystem/HexploreAttributeSet.h"

AHexplorePlayerState::AHexplorePlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UHexploreAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UHexploreAttributeSet>("AttributeSet");
	SetNetUpdateFrequency(100.f);
}

UAbilitySystemComponent* AHexplorePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
