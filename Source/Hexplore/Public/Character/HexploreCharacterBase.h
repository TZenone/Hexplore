// Copyright Tristan ZENONE.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/HexploreAttributeSet.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "HexploreCharacterBase.generated.h"

class UHexploreGameplayAbility;
class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS(Abstract)
class HEXPLORE_API AHexploreCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AHexploreCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; };

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetCombatTarget(AActor* CombatTarget);
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ClearCombatTarget();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	AActor* GetCombatTarget() const;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName BuffSocketName;

	virtual FVector GetBuffSocketLocation() override;

	/*
	 * GAS
	 */

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultRegenAttributes;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	void InitializeDefaultAttributes() const;
	void AddCharacterAbilities();
	
	/*
	 * Combat 
	 */

	void AttackSpeedChanged(const FOnAttributeChangeData& Data);
	void TryAutoAttack() const;
	
	/*
	* End Combat 
	*/
	
	
	
	/*
	 * End GAS
	 */

	/*
	 * Combat 
	 */

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnAttackSpeedChanged;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<UGameplayAbility> BasicAttackClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat")
	bool bIsInCombat = false;
	
	FTimerHandle AutoAttackTimerHandle;

	/*
	* End Combat 
	*/

private:
	
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
};
