// Copyright Tristan ZENONE.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/HexploreAttributeSet.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "HexploreCharacterBase.generated.h"

class USphereComponent;
class UHexploreGameplayAbility;
class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;
class UAnimMontage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEngagementSignature, AActor*, Actor);

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
	virtual void ClearCombatTarget();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	AActor* GetCombatTarget() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void OpportunityAction();

	virtual void Die() override;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

	virtual UAnimMontage* GetHitReactMontage_Implementation() override;

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
	 * End GAS
	 */
	
	/*
	 * Combat 
	 */

	void EngageTarget(AActor* TargetToEngage);
	
	void DisengageTarget(AActor* TargetToDisengage);

	UFUNCTION()
	virtual void OnEngaged(AActor* Target);

	UFUNCTION()
	virtual void OnDisengaged(AActor* Target);

	UFUNCTION()
	void OnEngagementRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEngagementRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetWeaponSheathed(bool bSheathed);
	
	/*
	* End Combat 
	*/
	

	/*
	 * Combat 
	 */

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<UAnimInstance> SheatheWeaponMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<UAnimInstance> UnsheatheWeaponMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(VisibleAnywhere, Category = "Combat | Engagement")
	TObjectPtr<USphereComponent> EngagementRange;	

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<UGameplayAbility> BasicAttackClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<UGameplayAbility> OpportunityActionClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat")
	TObjectPtr<AActor> EngagedTarget;

	FEngagementSignature EngagedTargetAdded;
	FEngagementSignature EngagedTargetRemoved;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat")
	bool bIsInCombat = false;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Combat")
	float EngagementRadius = 200.f;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Combat")
	float EngagementMovementSpeed = 150.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat")
	bool bWeaponIsSheathed = true;

	/*
	* End Combat 
	*/

private:
	
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
};
