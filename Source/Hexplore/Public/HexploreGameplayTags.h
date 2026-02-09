// Copyright Tristan ZENONE.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * HexploreGameplayTags
 *
 * Singleton containing native GameplayTags
 */

struct FHexploreGameplayTags
{
public:

	static const FHexploreGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	// Vital Attributes

	// Primary Attributes
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Dexterity;
	FGameplayTag Attributes_Primary_Endurance;
	FGameplayTag Attributes_Primary_Vigor;
	
	// Secondary Attributes
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPen;
	FGameplayTag Attributes_Secondary_Block;
	FGameplayTag Attributes_Secondary_CritChance;
	FGameplayTag Attributes_Secondary_CritMulti;
	FGameplayTag Attributes_Secondary_HealthRegen;
	FGameplayTag Attributes_Secondary_StaminaRegen;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxStamina;
	
protected:

private:
	static FHexploreGameplayTags GameplayTags;
};
