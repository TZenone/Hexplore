// Copyright Tristan ZENONE.


#include "HexploreGameplayTags.h"
#include "GameplayTagsManager.h"

FHexploreGameplayTags FHexploreGameplayTags::GameplayTags;

void FHexploreGameplayTags::InitializeNativeGameplayTags()
{
	// Primary Attributes
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"),
		FString("Increases physical damage.")
		);
	GameplayTags.Attributes_Primary_Dexterity = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Dexterity"),
		FString("Increases .")
		);
	GameplayTags.Attributes_Primary_Endurance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Endurance"),
		FString("Increases maximum Stamina.")
		);
	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Vigor"),
		FString("Increases Health and Stamina regeneration.")
		);

	// Secondary Attributes
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"),
		FString("Reduces damage taken.")
		);
	GameplayTags.Attributes_Secondary_ArmorPen = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ArmorPen"),
		FString("Reduces enemy armor.")
		);
	GameplayTags.Attributes_Secondary_Block = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Block"),
		FString("Chance to block a hit.")
		);
	GameplayTags.Attributes_Secondary_CritChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CritChance"),
		FString("Chance to land a critical strike.")
		);
	GameplayTags.Attributes_Secondary_CritMulti = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CritMulti"),
		FString("Critical damage multiplier.")
		);
	GameplayTags.Attributes_Secondary_HealthRegen = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegen"),
		FString("Health regeneration per second.")
		);
	GameplayTags.Attributes_Secondary_StaminaRegen = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.StaminaRegen"),
		FString("Stamina regeneration per second.")
		);
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"),
		FString("Maximum health.")
		);
	GameplayTags.Attributes_Secondary_MaxStamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxStamina"),
		FString("Maximum stamina.")
		);
	GameplayTags.Attributes_Secondary_AttackSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.AttackSpeed"),
		FString("Attack speed.")
		);

	// Input Tags
	GameplayTags.Input_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.LMB"),
		FString("Input Tag for left mouse button.")
		);
	GameplayTags.Input_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.RMB"),
		FString("Input Tag for right mouse button.")
		);
	GameplayTags.Input_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.1"),
		FString("Input Tag for 1 Key.")
		);
	GameplayTags.Input_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.2"),
		FString("Input Tag for 2 Key.")
		);
	GameplayTags.Input_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.3"),
		FString("Input Tag for 3 Key.")
		);
	GameplayTags.Input_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.4"),
		FString("Input Tag for 4 Key.")
		);
	GameplayTags.Input_5 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.5"),
		FString("Input Tag for 5 Key.")
		);
	GameplayTags.Input_E = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.E"),
		FString("Input Tag for E Key.")
		);
	GameplayTags.Input_Q = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Q"),
		FString("Input Tag for Q Key.")
		);
	GameplayTags.Input_R = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.R"),
		FString("Input Tag for R Key.")
		);
	GameplayTags.Input_T = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.T"),
		FString("Input Tag for T Key.")
		);
	GameplayTags.Input_F = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.F"),
		FString("Input Tag for F Key.")
		);
	GameplayTags.Input_G = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.G"),
		FString("Input Tag for G Key.")
		);
	GameplayTags.Input_C = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.C"),
		FString("Input Tag for C Key.")
		);
	GameplayTags.Input_V = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.V"),
		FString("Input Tag for V Key.")
		);

	// Status Tags
	GameplayTags.Status_General_Fleeing = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.General.Fleeing"),
		FString("Status tag for fleeing.")
		);
	GameplayTags.Status_General_ImmuneToOpportunityAction = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.General.ImmuneToOpportunityAction"),
		FString("Status tag for being immune to opportunity actions.")
		);
	
}
