// Copyright Tristan ZENONE.

#pragma once

#include "CoreMinimal.h"
#include "Character/HexploreCharacterBase.h"
#include "Interaction/TargetInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"

#include "HexploreEnemy.generated.h"

class UWidgetComponent;
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

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
	
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
};
