// Copyright Tristan ZENONE.


#include "AbilitySystem/HexploreAttributeSet.h"
#include "Net/UnrealNetwork.h"

UHexploreAttributeSet::UHexploreAttributeSet()
{
	InitMaxHealth(100.f);
	InitHealth(50.f);
	InitMaxStamina(100.f);
	InitStamina(50.f);
}

void UHexploreAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UHexploreAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHexploreAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHexploreAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHexploreAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
}

void UHexploreAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHexploreAttributeSet, Health, OldHealth);
}

void UHexploreAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHexploreAttributeSet, MaxHealth, OldMaxHealth);
}

void UHexploreAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHexploreAttributeSet, Stamina, OldStamina);
}

void UHexploreAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHexploreAttributeSet, MaxStamina, OldMaxStamina);
}
