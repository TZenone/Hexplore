// Copyright Tristan ZENONE.

#pragma once

#include "CoreMinimal.h"
#include "Character/HexploreCharacterBase.h"
#include "HexploreCharacter.generated.h"

/**
 * 
 */
UCLASS()
class HEXPLORE_API AHexploreCharacter : public AHexploreCharacterBase
{
	GENERATED_BODY()

public:
	
	AHexploreCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

private:
	virtual void InitAbilityActorInfo() override;
};
