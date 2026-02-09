// Copyright Tristan ZENONE.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/HexploreGameplayAbility.h"
#include "HexploreBuffSpell.generated.h"

class AHexploreBuff;
/**
 * 
 */
UCLASS()
class HEXPLORE_API UHexploreBuffSpell : public UHexploreGameplayAbility
{
	GENERATED_BODY()

protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AHexploreBuff> BuffClass;
};
