// Copyright Tristan ZENONE.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "HexploreGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class HEXPLORE_API UHexploreGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;
};
