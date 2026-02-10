// Copyright Tristan ZENONE.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/HexploreGameplayAbility.h"
#include "HexploreAttack.generated.h"

class AHexploreMelee;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class HEXPLORE_API UHexploreAttack : public UHexploreGameplayAbility
{
	GENERATED_BODY()

protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	void SpawnAttack(const FVector& TargetLocation);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AHexploreMelee> MeleeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
};
