// Copyright Tristan ZENONE.

#pragma once

#include "CoreMinimal.h"
#include "Character/HexploreCharacterBase.h"
#include "Interaction/TargetInterface.h"
#include "HexploreEnemy.generated.h"

/**
 * 
 */
UCLASS()
class HEXPLORE_API AHexploreEnemy : public AHexploreCharacterBase, public ITargetInterface
{
	GENERATED_BODY()

public:
	AHexploreEnemy();

	/* Enemy Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/* End Enemy Interface */
	
	/* Combat Interface */
	/* End Combat Interface */

	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
};
