// Copyright Tristan ZENONE.

#pragma once

#include "CoreMinimal.h"
#include "Character/HexploreCharacterBase.h"
#include "Interaction/TargetInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"

#include "HexploreEnemy.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class AHexploreAIController;

/**
 * 
 */
UCLASS()
class HEXPLORE_API AHexploreEnemy : public AHexploreCharacterBase, public ITargetInterface
{
	GENERATED_BODY()

public:
	AHexploreEnemy();
	virtual void PossessedBy(AController* NewController) override;

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

	virtual void OnEngaged(AActor* Target) override;

	virtual void OnDisengaged(AActor* Target) override;
	
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	bool bHitReacting = false;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AHexploreAIController> HexploreAIController;
	
};
