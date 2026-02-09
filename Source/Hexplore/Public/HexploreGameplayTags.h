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

	// Input
	FGameplayTag Input_LMB;
	FGameplayTag Input_RMB;
	FGameplayTag Input_1;
	FGameplayTag Input_2;
	FGameplayTag Input_3;
	FGameplayTag Input_4;
	FGameplayTag Input_5;
	FGameplayTag Input_E;
	FGameplayTag Input_Q;
	FGameplayTag Input_R;
	FGameplayTag Input_T;
	FGameplayTag Input_F;
	FGameplayTag Input_G;
	FGameplayTag Input_C;
	FGameplayTag Input_V;
	
protected:

private:
	static FHexploreGameplayTags GameplayTags;
};
