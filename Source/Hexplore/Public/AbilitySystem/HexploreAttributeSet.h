// Copyright Tristan ZENONE.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "HexploreAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){}

	FGameplayEffectContextHandle EffectContextHandle;
	
	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};

/**
 * 
 */
UCLASS()
class HEXPLORE_API UHexploreAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UHexploreAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	/*
	 *  Vital Attributes
	 */
	
	UPROPERTY(BlueprintReadOnly, Replicated, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UHexploreAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Replicated, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UHexploreAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Replicated, ReplicatedUsing = OnRep_Stamina, Category = "Vital Attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UHexploreAttributeSet, Stamina);

	UPROPERTY(BlueprintReadOnly, Replicated, ReplicatedUsing = OnRep_MaxStamina, Category = "Vital Attributes")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UHexploreAttributeSet, MaxStamina);

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldStamina) const;

	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const;

	/*
	 *  Primary Attributes
	 */
	
	UPROPERTY(BlueprintReadOnly, Replicated, ReplicatedUsing = OnRep_Strength, Category = "Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UHexploreAttributeSet, Strength);

	UPROPERTY(BlueprintReadOnly, Replicated, ReplicatedUsing = OnRep_Dexterity, Category = "Primary Attributes")
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UHexploreAttributeSet, Dexterity);

	UPROPERTY(BlueprintReadOnly, Replicated, ReplicatedUsing = OnRep_Vigor, Category = "Primary Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UHexploreAttributeSet, Vigor);

	UPROPERTY(BlueprintReadOnly, Replicated, ReplicatedUsing = OnRep_Endurance, Category = "Primary Attributes")
	FGameplayAttributeData Endurance;
	ATTRIBUTE_ACCESSORS(UHexploreAttributeSet, Endurance);

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;
	
	UFUNCTION()
	void OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const;
	
	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

	UFUNCTION()
	void OnRep_Endurance(const FGameplayAttributeData& OldEndurance) const;

	/*
	 *  Secondary Attributes
	 */

	UPROPERTY(BlueprintReadOnly, Replicated, ReplicatedUsing = OnRep_Armor, Category = "Secondary Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UHexploreAttributeSet, Armor);

	UPROPERTY(BlueprintReadOnly, Replicated, ReplicatedUsing = OnRep_ArmorPen, Category = "Secondary Attributes")
	FGameplayAttributeData ArmorPen;
	ATTRIBUTE_ACCESSORS(UHexploreAttributeSet, ArmorPen);

	UPROPERTY(BlueprintReadOnly, Replicated, ReplicatedUsing = OnRep_Block, Category = "Secondary Attributes")
	FGameplayAttributeData Block;
	ATTRIBUTE_ACCESSORS(UHexploreAttributeSet, Block);

	UPROPERTY(BlueprintReadOnly, Replicated, ReplicatedUsing = OnRep_CritChance, Category = "Secondary Attributes")
	FGameplayAttributeData CritChance;
	ATTRIBUTE_ACCESSORS(UHexploreAttributeSet, CritChance);

	UPROPERTY(BlueprintReadOnly, Replicated, ReplicatedUsing = OnRep_CritMultiplier, Category = "Secondary Attributes")
	FGameplayAttributeData CritMultiplier;
	ATTRIBUTE_ACCESSORS(UHexploreAttributeSet, CritMultiplier);

	UPROPERTY(BlueprintReadOnly, Replicated, ReplicatedUsing = OnRep_HealthRegen, Category = "Secondary Attributes")
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(UHexploreAttributeSet, HealthRegen);

	UPROPERTY(BlueprintReadOnly, Replicated, ReplicatedUsing = OnRep_StaminaRegen, Category = "Secondary Attributes")
	FGameplayAttributeData StaminaRegen;
	ATTRIBUTE_ACCESSORS(UHexploreAttributeSet, StaminaRegen);

	UPROPERTY(BlueprintReadOnly, Replicated, ReplicatedUsing = OnRep_AttackSpeed, Category = "Secondary Attributes")
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UHexploreAttributeSet, AttackSpeed);
	
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;

	UFUNCTION()
	void OnRep_ArmorPen(const FGameplayAttributeData& OldArmorPen) const;

	UFUNCTION()
	void OnRep_Block(const FGameplayAttributeData& OldBlock) const;

	UFUNCTION()
	void OnRep_CritChance(const FGameplayAttributeData& OldCritChance) const;

	UFUNCTION()
	void OnRep_CritMultiplier(const FGameplayAttributeData& OldCritMultiplier) const;

	UFUNCTION()
	void OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen) const;

	UFUNCTION()
	void OnRep_StaminaRegen(const FGameplayAttributeData& OldStaminaRegen) const;

	UFUNCTION()
	void OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed) const;
	

private:

	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
};


